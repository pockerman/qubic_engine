#ifndef ITERATIVE_POLICY_EVALUATION_H
#define ITERATIVE_POLICY_EVALUATION_H

#include "cubic_engine/rl/algorithms/dp/dp_algo_base.h"
#include "cubic_engine/rl/worlds/discrete_world.h"
#include "cubic_engine/rl/policies/discrete_policy_base.h"


#include <cmath>
#include <memory>

namespace cengine{
namespace rl{

namespace policies {
class DiscretePolicyBase;
}

namespace algos {
namespace dp {

///
/// \brief The IterativePolicyEval class
///
template<typename TimeStepTp>
class IterativePolicyEval: public DPAlgoBase<TimeStepTp>
{
public:

    ///
    /// \brief env_t
    ///
    typedef typename DPAlgoBase<TimeStepTp>::env_t env_t;

    ///
    /// \brief IterativePolicyEval
    ///
    IterativePolicyEval(uint_t n_max_itrs, real_t tolerance, real_t gamma,
                        env_t& env, std::shared_ptr<policies::DiscretePolicyBase> policy);

    ///
    /// \brief step
    ///
    virtual void step()override final;

    ///
    /// \brief set_policy_from
    ///
    void set_policy_from(const policies::DiscretePolicyBase& policy_data){*policy_ = policy_data;}

    ///
    /// \brief policy
    /// \return
    ///
    const policies::DiscretePolicyBase& policy()const{return  *policy_;}

    ///
    /// \brief policy
    /// \return
    ///
    policies::DiscretePolicyBase& policy(){return  *policy_;}

protected:

    ///
    /// \brief policy_
    ///
    std::shared_ptr<policies::DiscretePolicyBase> policy_;

};

template<typename TimeStepTp>
IterativePolicyEval<TimeStepTp>::IterativePolicyEval(uint_t n_max_itrs, real_t tolerance, real_t gamma,
                                         env_t& env, std::shared_ptr<policies::DiscretePolicyBase> policy)
    :
      DPAlgoBase<TimeStepTp>(n_max_itrs, tolerance, gamma, env),
      policy_(policy)
{}

template<typename TimeStepTp>
void
IterativePolicyEval<TimeStepTp>::step(){

    auto delta = 0.0;

    for(uint_t s=0; s<this->env_ref_().n_states(); ++s){

        auto old_v = this->value_func()[s];

        auto new_v = 0.0;

        auto state_actions_probs = policy_->operator[](s);

        for(const auto& action_prob : state_actions_probs){

            auto aidx = action_prob.first;
            auto action_p = action_prob.second;

            auto transition_dyn = this->env_ref_().transition_dynamics(s, aidx);

            for(auto& dyn: transition_dyn){
                auto prob = dyn.probabilities[0];
                auto next_state = dyn.states[0];
                auto reward = dyn.rewards[0];
                auto done = dyn.dones[0];

                new_v += action_p * prob * (reward + this->gamma() * this->value_func()[next_state]);

            }
        }

        delta = std::max(delta, std::fabs(old_v - new_v));
        this->value_func()[s] = new_v;
    }

    this->iter_controller_().update_residual(delta);

}

}

}
}
}

#endif // ITERATIVE_POLICY_EVALUATION_H
