#include "cubic_engine/rl/algorithms/td/expected_sarsa.h"
#include "cubic_engine/rl/worlds/discrete_world.h"


#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/cart_pole.h"
#include "gymfcpp/time_step.h"

#include <torch/torch.h>

#include <deque>
#include <iostream>


namespace example{

using cengine::real_t;
using cengine::uint_t;
using cengine::rl::envs::DiscreteWorldBase;
using cengine::rl::policies::EpsilonGreedyPolicy;
using cengine::rl::algos::td::ExpectedSARSA;



//
class CartPoleDQNImpl: public torch::nn::Module
{
public:

    //
    CartPoleDQNImpl(uint_t h, uint_t w, uint_t output);

    // forward
    torch::Tensor forward(torch::Tensor input);

private:

    torch::nn::Linear linear_;
    torch::nn::Conv2d conv1_;
    torch::nn::BatchNorm2d bn1_;
    torch::nn::Conv2d conv2_;
    torch::nn::BatchNorm2d bn2_;
    torch::nn::Conv2d conv3_;
    torch::nn::BatchNorm2d bn3_;

};


CartPoleDQN::CartPoleDQN(uint_t h, uint_t w, uint_t output_size)
    :
     torch::nn::Module()


}


int main(){

    using namespace example;

    try{

        /*Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWalkingEnv env(gym_namespace);
        env.build(true);

        std::cout<<"Number of states="<<env.n_states()<<std::endl;
        std::cout<<"Number of actions="<<env.n_actions()<<std::endl;

        EpsilonGreedyPolicy policy(0.005, env.n_actions(), EpsilonDecayOption::NONE);
        ExpectedSARSA<gymfcpp::TimeStep, EpsilonGreedyPolicy> expected_sarsa(5000, 1.0e-8,
                                                                             1.0, 0.01, 100, env, 1000, policy);

        expected_sarsa.do_verbose_output();

        std::cout<<"Starting training..."<<std::endl;
        auto train_result = expected_sarsa.train();

        std::cout<<train_result<<std::endl;
        std::cout<<"Finished training..."<<std::endl;

        std::cout<<"Saving value function..."<<std::endl;
        std::cout<<"Value function..."<<expected_sarsa.value_func()<<std::endl;
        expected_sarsa.save("expected_sarsa_value_func.csv");
        expected_sarsa.save_avg_scores("expected_sarsa_avg_scores.csv");
        expected_sarsa.save_state_action_function("expected_sarsa_state_action_function.csv");*/

    }
    catch(std::exception& e){
        std::cout<<e.what()<<std::endl;
    }
    catch(...){

        std::cout<<"Unknown exception occured"<<std::endl;
    }
    return 0;
}
