#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/a2c.h"
#include "cubic_engine/rl/policies/torch_policy.h"
#include "cubic_engine/rl/worlds/gym_lunar_lander_world.h"

#include "torch/torch.h"

#include <iostream>
#include <memory>
#include <string>

namespace example{


// Environment hyperparameters
const int num_envs = 1;
const std::string server_addr = "tcp://127.0.0.1:10201";
using cengine::rl::gym::Communicator;
using cengine::rl::worlds::GymLunarLanderWorld;
using cengine::rl::A2CInput;
using cengine::rl::A2C;
using cengine::rl::policies::TorchPolicy;

}


int main(){

    using namespace example;

    try{

        // set up the device
        torch::Device device(torch::kCPU);

#ifdef USE_LOG
        kernel::Logger::set_log_file_name("example_10_log.log");
#endif

        // create the communicator
        Communicator communicator(server_addr);

        // the environment
        GymLunarLanderWorld environment("v2", communicator);
        environment.build(true);

        A2CInput agend_input;
        TorchPolicy policy;
        A2C<GymLunarLanderWorld> agend(environment, policy, agend_input);

        // train the agend
        agend.train();


    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
