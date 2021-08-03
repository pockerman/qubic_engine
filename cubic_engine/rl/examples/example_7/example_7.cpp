#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/algorithms/dp/policy_iteration.h"
#include "cubic_engine/rl/worlds/discrete_world.h"
#include "cubic_engine/rl/policies/uniform_discrete_policy.h"
#include "cubic_engine/rl/policies/stochastic_adaptor_policy.h"

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/frozen_lake.h"
#include "gymfcpp/time_step.h"

#include <boost/python.hpp>

#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>
#include <random>
#include <algorithm>

namespace exe
{

using cengine::real_t;
using cengine::uint_t;
using cengine::rl::envs::DiscreteWorldBase;
using cengine::rl::policies::UniformDiscretePolicy;
using cengine::rl::policies::StochasticAdaptorPolicy;
using cengine::rl::algos::dp::PolicyIteration;



class FrozenLakeEnv: public DiscreteWorldBase<gymfcpp::TimeStep>
{

public:

    typedef DiscreteWorldBase<gymfcpp::TimeStep>::action_t action_t;
    typedef DiscreteWorldBase<gymfcpp::TimeStep>::time_step_t time_step_t;

    //
    FrozenLakeEnv(gymfcpp::obj_t gym_namespace);

    ~FrozenLakeEnv() = default;

    virtual uint_t n_actions()const override final {return env_impl_.n_actions();}
    virtual uint_t n_states()const override final {return env_impl_.n_states();}
    virtual std::vector<std::tuple<real_t, uint_t, real_t, bool>> transition_dynamics(uint_t s, uint_t aidx)const override final;

    virtual time_step_t step(const action_t&)override final {return time_step_t();}

    virtual time_step_t reset() override final;
    virtual  void build(bool reset) override final;
    virtual uint_t n_copies()const override final{return 1;}

private:

    // the environment implementation
    gymfcpp::FrozenLake env_impl_;

};

FrozenLakeEnv::FrozenLakeEnv(gymfcpp::obj_t gym_namespace)
    :
     DiscreteWorldBase<gymfcpp::TimeStep>("FrozenLake"),
     env_impl_("v0", gym_namespace)
{}


FrozenLakeEnv::time_step_t
FrozenLakeEnv::reset(){
    return env_impl_.reset();
}

void
FrozenLakeEnv::build(bool reset){
    env_impl_.make();
}

std::vector<std::tuple<real_t, uint_t, real_t, bool>>
FrozenLakeEnv::transition_dynamics(uint_t s, uint_t aidx)const{
    return env_impl_.p(s, aidx);
}

}

int main() {

    using namespace exe;

    Py_Initialize();
    auto gym_module = boost::python::import("gym");
    auto gym_namespace = gym_module.attr("__dict__");

    FrozenLakeEnv env(gym_namespace);
    env.build(true);

    auto policy = std::make_shared<UniformDiscretePolicy>(env.n_states(), env.n_actions());

    std::cout<<"Policy before training..."<<std::endl;
    std::cout<<*policy<<std::endl;

    auto policy_adaptor = std::make_shared<StochasticAdaptorPolicy>(env.n_states(), env.n_actions(), policy);

    PolicyIteration<gymfcpp::TimeStep> policy_itr(100, 1.0e-8, env, 1.0, 100,
                                                        policy, policy_adaptor);
    policy_itr.train();

    std::cout<<"Policy after training..."<<std::endl;
    std::cout<<*policy<<std::endl;

    // save the value function into a csv file
    policy_itr.save("policy_iteration.csv");

    return 0;
}


