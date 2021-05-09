#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/discrete_gym_world.h"
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
using cengine::rl::worlds::DiscreteGymWorld;
using cengine::rl::SyncValueFuncItr;
using cengine::rl::SyncValueFuncItrInput;
}


int main(){


    using namespace example;

    try{

#ifdef USE_LOG
        kernel::Logger::set_log_file_name("example_8_log.xtx");
#endif

        // create the communicator
        Communicator communicator(server_addr);

        DiscreteGymWorld world(communicator);
        world.build(env_name);

        std::cout<<"Number of states="<<world.n_states()<<std::endl;
        std::cout<<"Number of actions="<<world.n_actions()<<std::endl;

        SyncValueFuncItrInput input;

        SyncValueFuncItr<DiscreteGymWorld> value_function(input);
        value_function.initialize(world, 0.0);
        value_function.train();


        /*auto make_param = std::make_shared<MakeParam>();

        make_param->env_name = env_name;
        make_param->num_envs = num_envs;

        Request<MakeParam> make_request("make", make_param);
        communicator.send_request(make_request);

        Request<InfoParam> info_request("info", std::make_shared<InfoParam>());
        communicator.send_request(info_request);
        auto env_info = communicator.get_response<InfoResponse>();

        auto reset_param = std::make_shared<ResetParam>();
        Request<ResetParam> reset_request("reset", reset_param);
        communicator.send_request(reset_request);

        auto observation_shape = env_info->observation_space_shape;

        for(uint_t itrs = 0; itrs < 100; ++itrs){

        }*/

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
