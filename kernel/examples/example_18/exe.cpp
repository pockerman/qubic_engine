#include "kernel/base/types.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/threading/stoppable_task.h"
#include "kernel/vehicles/difd_drive_vehicle.h"


#include <iostream>
#include <mutex>

namespace example
{

std::mutex read_cmd_mutex;

using kernel::real_t;
using kernel::uint_t;
using DynMat = kernel::DynMat<real_t>;
using DynVec = kernel::DynVec<real_t>;
using kernel::ThreadPool;
using kernel::DiffDriveVehicle;

const real_t DT = 0.5;

struct StopSimulation
{
    bool stop()const{return condition; }
    void set_condition(bool cond){condition=cond;}
    bool condition{false};
};

struct SimData
{
    std::pair<real_t, real_t> get_data()const;
    void set_data(std::pair<real_t, real_t>&& data);

    mutable std::mutex sim_data_mutex;
    std::pair<real_t, real_t> data;
    bool exit{false};
};

std::pair<real_t, real_t>
SimData::get_data()const{

    std::unique_lock<std::mutex> lock(sim_data_mutex);
    return data;
}

void
SimData::set_data(std::pair<real_t, real_t>&& data){
   std::unique_lock<std::mutex> lock(sim_data_mutex);
   data = data;
}

class Server: public kernel::StoppableTask<StopSimulation>
{
public:

    Server(const StopSimulation& stop_condition, SimData& simdata);

protected:

    virtual void run()override final;

    SimData& sdata;
};

Server::Server(const StopSimulation& stop_condition, SimData& simdata)
    :
  kernel::StoppableTask<StopSimulation>(stop_condition),
  sdata(simdata)
{}

void
Server::run(){


    std::string cmd;
    while(!this->should_stop()){

        std::cout<<"Enter CMD for robot:..."<<std::endl;

        // Read the CMD
        std::getline(std::cin, cmd);

        if(cmd == "EXIT"){
            std::cout<<"Exit simulation loop..."<<std::endl;
            this->get_condition().set_condition(true);
        }
        else{
           std::cout<<"Command entered: "<<cmd<<std::endl;
        }
    }
}

class Simulator: public kernel::StoppableTask<StopSimulation>
{

public:

   Simulator(const StopSimulation& stop_condition );

protected:

    virtual void run()override final;

   // the vehicle the simulator is simulating
   DiffDriveVehicle vehicle_;
};

Simulator::Simulator(const StopSimulation& stop_condition)
    :
   kernel::StoppableTask<StopSimulation>(stop_condition),
   vehicle_(2.5)
{}

void
Simulator::run(){

    uint_t sim_counter = 0;
    while (!this->should_stop()) {





    }

}

}


int main(){

    using namespace example;

    StopSimulation stop_sim;
    SimData simdata;


    ThreadPool pool(2);

    Server server(stop_sim, simdata);

    pool.add_task(server);

    Simulator simulator(stop_sim);
    //pool.add_task(simulator);

    //this should block
    pool.close();

    return 0;
}




