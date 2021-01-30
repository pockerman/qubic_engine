#include "kernel/base/types.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/threading/stoppable_task.h"
#include "kernel/parallel/data_structs/lockable_queue.h"
#include "kernel/vehicles/difd_drive_vehicle.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/utilities/csv_file_writer.h"


#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

namespace example
{

std::mutex msg_mutex;

using kernel::real_t;
using kernel::uint_t;
using DynMat = kernel::DynMat<real_t>;
using DynVec = kernel::DynVec<real_t>;
using kernel::ThreadPool;
using kernel::ThreadPoolOptions;
using kernel::DiffDriveVehicle;
using kernel::DiffDriveConfig;
using kernel::LockableQueue;
using kernel::GeomPoint;

const real_t DT = 0.5;

struct StopSimulation
{
    bool stop()const{return condition; }
    void set_condition(bool cond){condition=cond;}
    bool condition{false};
};


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


class DiffDriveVehicleWrapper
{

public:

    DiffDriveVehicleWrapper(DiffDriveVehicle& vehicle);

    /// set the CMD to execute
    void set_cmd(CMD cmd);

    /// \brief integrate the diff drive system
    /// account for errors also
    std::string integrate();

    /// \brief Returns the state of the vehicle as string
    std::string get_state_as_string()const;

    /// \brief Returns the state of the vehicle as string
    std::tuple<real_t, real_t> get_position()const;

    /// \brief Returns the state of the vehicle as string
    std::tuple<real_t, real_t, real_t, real_t> get_state()const;

private:

    mutable std::mutex integrate_mutex_;
    DiffDriveVehicle* vehicle_ptr_;
    CMD cmd_;
};

DiffDriveVehicleWrapper::DiffDriveVehicleWrapper(DiffDriveVehicle& vehicle)
    :
   integrate_mutex_(),
   vehicle_ptr_(&vehicle),
   cmd_()
{}

std::string
DiffDriveVehicleWrapper::integrate(){
    std::lock_guard<std::mutex> lock(integrate_mutex_);

    if(cmd_.name != "INVALID_NAME"){
        vehicle_ptr_->integrate(cmd_.velocity, cmd_.orientation);
    }

    return cmd_.name;
}

std::string
DiffDriveVehicleWrapper::get_state_as_string()const{

    std::lock_guard<std::mutex> lock(integrate_mutex_);

    auto x = vehicle_ptr_->get_x_position();
    auto y = vehicle_ptr_->get_y_position();
    auto theta = vehicle_ptr_->get_orientation();
    auto velocity = vehicle_ptr_->get_velocity();
    return std::to_string(x)+","+std::to_string(y)+","+std::to_string(theta)+","+std::to_string(velocity)+"\n";

}

std::tuple<real_t, real_t>
DiffDriveVehicleWrapper::get_position()const{

    std::lock_guard<std::mutex> lock(integrate_mutex_);

    auto x = vehicle_ptr_->get_x_position();
    auto y = vehicle_ptr_->get_y_position();
    return {x,y};
}

std::tuple<real_t, real_t, real_t, real_t>
DiffDriveVehicleWrapper::get_state()const{

    auto x = vehicle_ptr_->get_x_position();
    auto y = vehicle_ptr_->get_y_position();
    auto theta = vehicle_ptr_->get_orientation();
    auto velocity = vehicle_ptr_->get_velocity();

    return {x, y, theta, velocity};

}

void
DiffDriveVehicleWrapper::set_cmd(CMD cmd){
    std::lock_guard<std::mutex> lock(integrate_mutex_);
    cmd_ = cmd;
}


class ServerThread: public kernel::StoppableTask<StopSimulation>
{
public:

    ServerThread(const StopSimulation& stop_condition,
                 DiffDriveVehicleWrapper& vwrapper,
                 ThreadPool& threads);

protected:

    virtual void run()override final;

    DiffDriveVehicleWrapper& vwrapper_;

    LockableQueue<std::string> requests_;
    LockableQueue<std::string> responses_;
    LockableQueue<CMD> cmds_;


    ThreadPool& thread_pool_;

    struct SimulatorTask;
    struct RequestTask;
    struct ClientTask;

    // list of tasks the server handles
    std::vector<std::unique_ptr<kernel::TaskBase>> tasks_;

    // checks if all tasks are stopped
    // if this is true the server is stopped as well
    void tasks_stopped();

};

ServerThread::ServerThread(const StopSimulation& stop_condition,
                           DiffDriveVehicleWrapper& vwrapper,
                           ThreadPool& thread_pool)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  vwrapper_(vwrapper),
  requests_(),
  responses_(),
  cmds_(),
  thread_pool_(thread_pool),
  tasks_()
{
    this->set_name("ServerThread");
}

struct ServerThread::SimulatorTask: public kernel::StoppableTask<StopSimulation>
{

public:

   SimulatorTask(const StopSimulation& stop_condition, DiffDriveVehicleWrapper& vwrapper );

protected:

    virtual void run()override final;
    DiffDriveVehicleWrapper& vwrapper_;

};

ServerThread::SimulatorTask::SimulatorTask(const StopSimulation& stop_condition, DiffDriveVehicleWrapper& vwrapper)
    :
   kernel::StoppableTask<StopSimulation>(stop_condition),
   vwrapper_(vwrapper)
{
    this->set_name("SimulatorTask");
}

void
ServerThread::SimulatorTask::run(){

    // run as long as we are not told to stop
    while (!this->should_stop()) {

        auto cmd = vwrapper_.integrate();

        if(cmd == "EXIT"){
            this->get_condition().set_condition(true);
        }
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}


struct ServerThread::RequestTask: public kernel::StoppableTask<StopSimulation>
{

public:

    RequestTask(const StopSimulation& stop_condition,
                LockableQueue<std::string>& requests,
                LockableQueue<CMD>& cmds,
                LockableQueue<std::string>& responses,
                DiffDriveVehicleWrapper& vwrapper);

protected:

    virtual void run()override final;
    LockableQueue<std::string>& requests_;
    LockableQueue<CMD>& cmds_;
    LockableQueue<std::string>& responses_;
    DiffDriveVehicleWrapper& vwrapper_;
    GeomPoint<2> position_;

    void serve_request(const std::string& request);
    void save_solution(kernel::CSVWriter& writer);
};

ServerThread::RequestTask::RequestTask(const StopSimulation& stop_condition,
                                       LockableQueue<std::string>& requests,
                                       LockableQueue<CMD>& cmds,
                                       LockableQueue<std::string>& responses,
                                       DiffDriveVehicleWrapper& vwrapper)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  requests_(requests),
  cmds_(cmds),
  responses_(responses),
  vwrapper_(vwrapper)
{
    this->set_name("RequestTask");
}

void
ServerThread::RequestTask::run(){

    // the object that handles the solution output
    kernel::CSVWriter writer("vehicle_state.csv");


    while(!this->should_stop()){

        save_solution(writer);

        // otherwise create the input
        while(!requests_.empty()){

            auto request = requests_.pop_wait();

            // if the request is a CMD
            serve_request(request);
        }
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::RequestTask::serve_request(const std::string& request){

    std::vector<std::string> strings;
    std::istringstream f(request);
    std::string s;
    while (std::getline(f, s, ';')) {
        strings.push_back(s);
    }

    if(!strings.empty()){
        if(strings[0] == "CMD"){

            //we have a CMD
            if(strings[1] == "S"){
                auto [x, y, theta, velocity]  = vwrapper_.get_state();
                cmds_.push_item(CMD::generate_cmd(0.0, theta, strings[1]));
            }
            else if(strings[1] == "V"){

                    auto vel_str = strings[2];
                    real_t vel = std::atof(vel_str.c_str());

                    auto [x, y, theta, velocity]  = vwrapper_.get_state();
                    real_t w = theta;

                    if(strings.size() == 5 && strings[3] == "W" ){

                        auto w_str = strings[4];
                        w = std::atof(w_str.c_str());
                    }

                    cmds_.push_item(CMD::generate_cmd(vel*DT, w*DT, strings[1]));
            }
            else if(strings[1] == "W"){

                    auto w_str = strings[2];
                    real_t w = std::atof(w_str.c_str());

                    auto [x, y, theta, velocity]  = vwrapper_.get_state();
                    real_t v = velocity;

                    if(strings.size() == 5 && strings[3] == "V" ){

                        auto v_str = strings[4];
                        v = std::atof(v_str.c_str());
                    }

                    cmds_.push_item(CMD::generate_cmd(v*DT, w*DT, strings[1]));

            }
        }
        else if(strings[0] == "EXIT"){

            this->get_condition().set_condition(true);
            CMD exit(0.0, 0.0, "EXIT");
            cmds_.push_item(exit);
        }
        else if(strings[0] == "PRINT"){
            responses_.push_item(vwrapper_.get_state_as_string());
        }
    }
}

void
ServerThread::RequestTask::save_solution(kernel::CSVWriter& writer){


    auto [x, y]  = vwrapper_.get_position();
    std::vector<real_t> row(2);
    GeomPoint<2> current(std::vector<real_t>({x,y}));

    if(position_.distance(current) > 100.0){
        writer.open(std::ios_base::app);

        row[0] = x;
        row[1] = y;
        writer.write_row(row);
        writer.close();
        position_ = current;
    }
}

struct ServerThread::ClientTask: public kernel::StoppableTask<StopSimulation>
{

public:

    ClientTask(const StopSimulation& stop_condition,
               LockableQueue<std::string>& request,
               LockableQueue<std::string>& response);

protected:

    virtual void run()override final;

    LockableQueue<std::string>& requests_;
    LockableQueue<std::string>& responses_;
};

ServerThread::ClientTask::ClientTask(const StopSimulation& stop_condition,
                                     LockableQueue<std::string>& request,
                                     LockableQueue<std::string>& response)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  requests_(request),
  responses_(response)

{
    this->set_name("ClientTask");
}

void
ServerThread::ClientTask::run(){

    std::string request;
    while(!this->should_stop()){

        std::cout<<"MESSAGE: Enter CMD for robot..."<<std::endl;

        // Read the CMD
        std::getline(std::cin, request);

        std::cout<<"MESSAGE: CMD entered..."<<request<<std::endl;

        if(request == "EXIT"){

            this->get_condition().set_condition(true);
            requests_.push_item(request);
            break;
        }
        else{
            requests_.push_item(request);
        }

        while(!responses_.empty()){
            //empty what is comming from the server
            auto response = responses_.pop_wait();
            std::cout<<"RESPONSE: "<<response<<std::endl;
        }
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::run(){

    // create the tasks and assign them
    // to the queue
    tasks_.reserve(3);
    tasks_.push_back(std::make_unique<ServerThread::SimulatorTask>(this->get_condition(), vwrapper_));
    tasks_.push_back(std::make_unique<ServerThread::RequestTask>(this->get_condition(), requests_, cmds_, responses_, vwrapper_));
    tasks_.push_back(std::make_unique<ServerThread::ClientTask>(this->get_condition(), requests_, responses_));

    // add the tasks
    thread_pool_.add_tasks(tasks_);

    while(!this->should_stop()){

        while(!cmds_.empty()){
            CMD cmd = cmds_.pop_wait();
            vwrapper_.set_cmd(cmd);
        }

        tasks_stopped();
    }

    std::lock_guard<std::mutex> lock(msg_mutex);
    std::cout<<"MESSAGE: Task "+this->get_name()<<" exited simulation loop..."<<std::endl;
}

void
ServerThread::tasks_stopped(){

    bool stop = true;
    for(uint_t t=0; t<tasks_.size(); ++t){

        if(tasks_[t] && !dynamic_cast<StoppableTask<StopSimulation>*>(tasks_[t].get())->is_stopped()){
            stop = false;
            break;
        }
    }

    if(stop){
        this->get_condition().set_condition(true);
    }
}

}


int main(){


    using namespace example;

    const uint_t N_THREADS = 4;
    const real_t RADIUS = 2.0/100.0;

    /// the shared object that controls when to stop
    StopSimulation stop_sim;

    DiffDriveConfig properties;
    properties.R = RADIUS;

    /// the vehicle the simulator is simulating
    DiffDriveVehicle vehicle(properties);

    // a wrapper class to provide synchronization
    DiffDriveVehicleWrapper vehicle_wrapper(vehicle);

    ThreadPoolOptions options;
    options.n_threads = N_THREADS;
    options.msg_on_start_up = true;
    options.msg_on_shut_down = true;
    options.msg_when_adding_tasks = true;
    options.start_on_construction = true;

    // executor
    ThreadPool pool(options);

    // the server instance
    ServerThread server(stop_sim, vehicle_wrapper, pool);

    // add the server to the pool
    pool.add_task(server);

    //this should block
    pool.close();

    return 0;
}




