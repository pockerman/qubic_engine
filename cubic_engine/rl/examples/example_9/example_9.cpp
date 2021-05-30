#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/discrete_gym_world.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/q_learning.h"
#include "torch/torch.h"

#include <iostream>
#include <memory>
#include <string>

namespace example{


// Environment parameters
const std::string env_name = "CartPole-v0";
const int num_envs = 1;
const std::string server_addr = "tcp://127.0.0.1:10201";

using cengine::uint_t;
using cengine::rl::gym::Communicator;
using cengine::rl::worlds::DiscreteGymWorld;
using cengine::rl::QLearning;
using cengine::rl::QLearningInput;

}


int main(){

    using namespace example;

    try{


#ifdef USE_LOG
        kernel::Logger::set_log_file_name("example_9_log.log");
#endif

        // create the communicator
        Communicator communicator(server_addr);

        DiscreteGymWorld<uint_t> world(communicator);
        world.build(env_name);

        std::cout<<"Number of states="<<world.n_states()<<std::endl;
        std::cout<<"Number of actions="<<world.n_actions()<<std::endl;

        QLearningInput qinput;
        QLearning<DiscreteGymWorld<uint_t>> qlearner(qinput);
        qlearner.initialize(world, 0.0);

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
