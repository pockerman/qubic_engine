#include "cubic_engine/rl/algorithms/td/sarsa.h"
#include "cubic_engine/rl/worlds/discrete_world.h"
#include "cubic_engine/rl/policies/epsilon_greedy_policy.h"

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/cliff_world.h"
#include "gymfcpp/time_step.h"

#include <deque>
#include <iostream>

namespace example{

using cengine::real_t;
using cengine::uint_t;
using cengine::rl::envs::DiscreteWorldBase;
using cengine::rl::policies::EpsilonGreedyPolicy;
using cengine::rl::algos::td::Sarsa;


class CliffWalkingEnv: public DiscreteWorldBase<gymfcpp::TimeStep>
{
public:

    typedef  DiscreteWorldBase<gymfcpp::TimeStep>::time_step_t time_step_t;
    typedef  DiscreteWorldBase<gymfcpp::TimeStep>::action_t action_t;

    CliffWalkingEnv(gymfcpp::obj_t gym_namespace);
    ~CliffWalkingEnv()=default;

    virtual uint_t n_actions()const override final {return env_impl_.n_actions();}
    virtual uint_t n_copies()const override final{return 1;}
    virtual uint_t n_states()const override final {return env_impl_.n_states();}

    virtual std::vector<std::tuple<real_t, uint_t, real_t, bool>> transition_dynamics(uint_t s, uint_t aidx)const override final;
    virtual time_step_t step(const action_t&)override final;
    virtual time_step_t reset() override final;
    virtual  void build(bool reset) override final;

private:

    gymfcpp::CliffWorld env_impl_;

};

CliffWalkingEnv::CliffWalkingEnv(gymfcpp::obj_t gym_namespace)
    :
      DiscreteWorldBase<gymfcpp::TimeStep>("Cliffwalking"),
      env_impl_("v0", gym_namespace, false)
{}

std::vector<std::tuple<real_t, uint_t, real_t, bool>>
CliffWalkingEnv::transition_dynamics(uint_t s, uint_t aidx)const{
    return std::vector<std::tuple<real_t, uint_t, real_t, bool>>();
}

void
CliffWalkingEnv::build(bool reset_){
    env_impl_.make();

    if(reset_){
        reset();
    }
}

CliffWalkingEnv::time_step_t
CliffWalkingEnv::step(const action_t& action){
    return env_impl_.step(action);
}

CliffWalkingEnv::time_step_t
CliffWalkingEnv::reset(){
    return env_impl_.reset();
}

}

int main(){

    using namespace example;

    try{

        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWalkingEnv env(gym_namespace);
        env.build(true);

        std::cout<<"Number of states="<<env.n_states()<<std::endl;
        std::cout<<"Number of actions="<<env.n_actions()<<std::endl;

        EpsilonGreedyPolicy policy(1.0, env.n_actions(), true);
        Sarsa<gymfcpp::TimeStep, EpsilonGreedyPolicy> sarsa(5000, 1.0e-8,
                                                            1.0, 0.01, 100, env, 1000, policy);

        sarsa.do_verbose_output();

        std::cout<<"Starting training..."<<std::endl;
        auto train_result = sarsa.train();

        std::cout<<train_result<<std::endl;
        std::cout<<"Finished training..."<<std::endl;

        std::cout<<"Saving value function..."<<std::endl;
        std::cout<<"Value function..."<<sarsa.value_func()<<std::endl;
        sarsa.save("sarsa_value_func.csv");
        sarsa.save_avg_scores("sarsa_avg_scores.csv");
        sarsa.save_state_action_function("sarsa_state_action_function.csv");

    }
    catch(std::exception& e){
        std::cout<<e.what()<<std::endl;
    }
    catch(...){

        std::cout<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
