#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/worlds/gym_lunar_lander_world.h"

//#include "torch/torch.h"

#include <iostream>
#include <memory>
#include <string>

namespace example{


// Environment parameters
const int num_envs = 1;
const std::string server_addr = "tcp://127.0.0.1:10201";

using cengine::uint_t;
using cengine::rl::gym::Communicator;
using cengine::rl::worlds::GymLunarLanderWorld;

}


int main(){

    using namespace example;

    try{


#ifdef USE_LOG
        kernel::Logger::set_log_file_name("example_9_log.log");
#endif

        // create the communicator
        Communicator communicator(server_addr);

        GymLunarLanderWorld environment("v2", communicator);
        environment.build(true);

        std::cout<<"Environment name: "<<environment.name()<<std::endl;

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
