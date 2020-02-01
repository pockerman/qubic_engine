#include "kernel/base/types.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/threading/stoppable_task.h"
#include "kernel/parallel/data_structs/lockable_queue.h"
#include "kernel/vehicles/difd_drive_vehicle.h"


#include <iostream>
#include <mutex>
#include <thread>

namespace example
{

std::mutex read_cmd_mutex;

using kernel::real_t;
using kernel::uint_t;
using DynMat = kernel::DynMat<real_t>;
using DynVec = kernel::DynVec<real_t>;
using kernel::ThreadPool;
using kernel::DiffDriveVehicle;
using kernel::LockableQueue;

const real_t DT = 0.5;

struct StopSimulation
{
    bool stop()const{return condition; }
    void set_condition(bool cond){condition=cond;}
    bool condition{false};
};

enum class RequestType{CMD, PLOT, PRINT};

struct CMD
{
    real_t velocity{0.0};
    real_t orientation{0.0};
    std::string name{"INVALID_CMD"};

    CMD()=default;

    CMD(real_t v, real_t o, std::string n)
        :
          velocity(v),
          orientation(o),
          name(n)
    {}

    static CMD generate_cmd(real_t v, real_t o, std::string name){
        return CMD(v, o, name);
    }
};



class ServerThread;
class SimulatorThread;

class DiffDriveVehicleWrapper
{

public:

    DiffDriveVehicleWrapper(DiffDriveVehicle& vehicle);

    /// \brief Read the x-coordinate
    real_t get_x_position()const;

    /// \brief Read the y-coordinate
    real_t get_y_position()const;

    /// \brief Read the orientation
    real_t get_orientation()const;

    /// \brief Read current velocity of the vehicle
    real_t get_velcoty()const;

    /// set the CMD to execute
    void set_cmd(CMD cmd);

private:

    mutable std::mutex integrate_mutex_;
    DiffDriveVehicle* vehicle_ptr_;

    CMD cmd_;

    /// \brief integrate the diff drive system
    /// account for errors also
    void integrate();

    friend class SimulatorThread;

};

DiffDriveVehicleWrapper::DiffDriveVehicleWrapper(DiffDriveVehicle& vehicle)
    :
   integrate_mutex_(),
   vehicle_ptr_(&vehicle),
   cmd_()
{}

void
DiffDriveVehicleWrapper::integrate(){
    std::unique_lock<std::mutex> lock(integrate_mutex_);
    vehicle_ptr_->integrate(cmd_.velocity, cmd_.orientation);
}

void
DiffDriveVehicleWrapper::set_cmd(CMD cmd){
    std::unique_lock<std::mutex> lock(integrate_mutex_);
    cmd_ = cmd;
}



real_t
DiffDriveVehicleWrapper::get_x_position()const{
    std::unique_lock<std::mutex> lock(integrate_mutex_);
    return vehicle_ptr_->get_x_position();
}

real_t
DiffDriveVehicleWrapper::get_y_position()const{

    std::unique_lock<std::mutex> lock(integrate_mutex_);
    return vehicle_ptr_->get_y_position();
}

real_t
DiffDriveVehicleWrapper::get_orientation()const{
    std::unique_lock<std::mutex> lock(integrate_mutex_);
    return vehicle_ptr_->get_orientation();
}

real_t
DiffDriveVehicleWrapper::get_velcoty()const{

    std::unique_lock<std::mutex> lock(integrate_mutex_);
    return vehicle_ptr_->get_velcoty();
}

class ClientThread: public kernel::StoppableTask<StopSimulation>
{

public:

    ClientThread(StopSimulation& stop_condition,
                 LockableQueue<std::string>& input,
                 LockableQueue<std::string>& output);

protected:

    virtual void run()override final;

    LockableQueue<std::string>& input_;
    LockableQueue<std::string>& output_;
};

ClientThread::ClientThread(StopSimulation& stop_condition,
                           LockableQueue<std::string>& input,
                           LockableQueue<std::string>& output)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition)
  input_(input),
  output_(output)

{}


void
ClientThread::run(){

    std::string request;
    while(!this->should_stop()){

        std::cout<<"Enter CMD for robot:..."<<std::endl;

        // Read the CMD
        std::getline(std::cin, request);

        if(request == "EXIT"){
            std::cout<<"Exit simulation loop..."<<std::endl;
            this->get_condition().set_condition(true);
        }
        else{
           std::cout<<"Command entered: "<<request<<std::endl;
        }


    }

}

class RequestThread: public kernel::StoppableTask<StopSimulation>
{

public:

    RequestThread(StopSimulation& stop_condition, LockableQueue<std::string>& requests);

protected:

    virtual void run()override final;
    LockableQueue<std::string>& requests_;
    LockableQueue<CMD>& cmds_;

    void serve_request(const std::string& request);
};

RequestThread::RequestThread(StopSimulation& stop_condition,
                             LockableQueue<std::string>& requests,
                             LockableQueue<CMD>& cmds)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  requests_(requests)
  cmds_(cmds)
{}

void
RequestThread::run(){

    while(!this->should_stop()){

        while(requests_.empty()){
            std::this_thread::yield();
        }

        // otherwise create the input
        while(!requests_.empty()){

            auto* request = requests_.pop();

            // if the request is a CMD
            serve_request(*request);
        }
    }
}

void
RequestThread::serve_request(const std::string& request){

    std::vector<std::string> strings;
    std::istringstream f(request);
    std::string s;
    while (std::getline(f, s, ';')) {
        strings.push_back(s);
    }

    if(strings[0] == "CMD"){

        CMD cmd;
    }
    else if(strings[0] == "PLT"){

    }
    else if(strings[0] == "PRN"){

    }
}


class ResponseThread: public kernel::StoppableTask<StopSimulation>
{

public:

    ResponseThread(StopSimulation& stop_condition, LockableQueue<std::string>& input);

protected:

    virtual void run()override final;
    LockableQueue<std::string>& input_;
};

ResponseThread::ResponseThread(StopSimulation& stop_condition, LockableQueue<std::string>& input)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  input_(input)
{}

void
ResponseThread::run(){

    while(!this->should_stop()){

        while(input_.empty()){
            std::this_thread::yield();
        }

        // otherwise create the input
    }
}



class ServerThread: public kernel::StoppableTask<StopSimulation>
{
public:

    ServerThread(StopSimulation& stop_condition, DiffDriveVehicleWrapper& vwrapper);

protected:

    virtual void run()override final;

    DiffDriveVehicleWrapper& vwrapper_;

};

ServerThread::ServerThread(StopSimulation& stop_condition, DiffDriveVehicleWrapper& vwrapper)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  vwrapper_(vwrapper)
{}

void
ServerThread::run(){


    std::string request;
    while(!this->should_stop()){

        std::cout<<"Enter CMD for robot:..."<<std::endl;

        // Read the CMD
        std::getline(std::cin, request);

        if(request == "EXIT"){
            std::cout<<"Exit simulation loop..."<<std::endl;
            this->get_condition().set_condition(true);
        }
        else{
           std::cout<<"Command entered: "<<request<<std::endl;
        }

        CMD cmd = CMD::generate_cmd(0.0, 0.0, "EXIT");
        vwrapper_.set_cmd(cmd);
    }
}

class SimulatorThread: public kernel::StoppableTask<StopSimulation>
{

public:

   SimulatorThread(StopSimulation& stop_condition, DiffDriveVehicleWrapper& vwrapper );

protected:

    virtual void run()override final;
    DiffDriveVehicleWrapper& vwrapper_;

};

SimulatorThread::SimulatorThread(StopSimulation& stop_condition, DiffDriveVehicleWrapper& vwrapper)
    :
   kernel::StoppableTask<StopSimulation>(stop_condition),
   vwrapper_(vwrapper)
{}

void
SimulatorThread::run(){

    // run as long as we are not told to stop
    while (!this->should_stop()) {
        vwrapper_.integrate();
    }
}

}


int main(){


    using namespace example;

    const uint_t N_THREADS = 2;
    const real_t RADIUS = 2.0;

    // the queue over which the two threads will communicate
    LockableQueue<std::string> input_queue;
    LockableQueue<std::string> output_queue;

    // the shared object that
    // controls when to stop
    StopSimulation stop_sim;

    // the vehicle the simulator is simulating
    DiffDriveVehicle vehicle(RADIUS);

    // a wrapper class to provide synchronization
    DiffDriveVehicleWrapper vehicle_wrapper(vehicle);

    // executor
    ThreadPool pool(N_THREADS);

    // the server instance
    ServerThread server(stop_sim, vehicle_wrapper, input_queue, output_queue);

    pool.add_task(server);

    SimulatorThread simulator(stop_sim, vehicle_wrapper);
    pool.add_task(simulator);

    ClientThread client(stop_sim, input_queue, output_queue);
    pool.add_task(client);


    //this should block
    pool.close();

    return 0;
}




