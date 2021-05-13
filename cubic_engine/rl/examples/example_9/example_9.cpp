#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/discrete_gym_world.h"
#include "cubic_engine/rl/worlds/discrete_gym_world_with_dynamics.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/synchronous_value_function_learning.h"
#include "torch/torch.h"

#include <iostream>
#include <memory>
#include <string>

namespace example{


// Environment hyperparameters
const std::string env_name = "CartPole-v0";
const int num_envs = 1;
const std::string server_addr = "tcp://127.0.0.1:10201";
using cengine::rl::gym::Communicator;
using cengine::rl::worlds::DiscreteGymWorldWithDynamics;


// The Deep neural network
class DQN: public torch::nn::Module
{
public:

    // construct
    DQN();

    // forward
    torch::Tensor forward(torch::Tensor input);

private:


    torch::nn::Conv2d conv1;
    torch::nn::BatchNorm2d bn1;

    torch::nn::Conv2d conv2;
    torch::nn::BatchNorm2d bn2;

    torch::nn::Conv2d conv3;
    torch::nn::BatchNorm2d bn3;

};


DQN::DQN()
    :
    torch::nn::Module(),
    conv1(3, 16, 5, 2),
    bn1(16),
    conv2(16, 32, 5, 2),
    bn2(32),
    conv3(32, 32, 5, 2),
    bn3(32)
{}


}


int main(){

    using namespace example;

    try{

        // set up the device
        torch::Device device(torch::kCPU);

#ifdef USE_LOG
        kernel::Logger::set_log_file_name("example_9_log.log");
#endif

        // create the communicator
        Communicator communicator(server_addr);

        DiscreteGymWorldWithDynamics world(communicator);
        world.build(env_name);

        std::cout<<"Number of states="<<world.n_states()<<std::endl;
        std::cout<<"Number of actions="<<world.n_actions()<<std::endl;




    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
