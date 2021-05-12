#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/discrete_gym_world.h"
#include "cubic_engine/rl/worlds/discrete_gym_world_with_dynamics.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/synchronous_value_function_learning.h"

#include <iostream>
#include <memory>
#include <string>

namespace example{


// Environment hyperparameters
const std::string env_name = "FrozenLake-v0";
const int num_envs = 1;
const std::string server_addr = "tcp://127.0.0.1:10201";
using cengine::rl::gym::Communicator;
using cengine::rl::worlds::DiscreteGymWorldWithDynamics;
using cengine::rl::SyncValueFuncItr;
using cengine::rl::SyncValueFuncItrInput;
}


int main(){

    using namespace example;

    try{

#ifdef USE_LOG
        kernel::Logger::set_log_file_name("example_8_log.log");
#endif

        // create the communicator
        Communicator communicator(server_addr);

        DiscreteGymWorldWithDynamics world(communicator);
        world.build(env_name);

        std::cout<<"Number of states="<<world.n_states()<<std::endl;
        std::cout<<"Number of actions="<<world.n_actions()<<std::endl;

        SyncValueFuncItrInput input;
        input.tol = 1.0e-5;
        input.gamma = 1.0;
        input.n_iterations = 100;
        input.show_iterations = true;

        SyncValueFuncItr<DiscreteGymWorldWithDynamics> value_function(input);
        value_function.initialize(world, 0.0);
        value_function.train();
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
