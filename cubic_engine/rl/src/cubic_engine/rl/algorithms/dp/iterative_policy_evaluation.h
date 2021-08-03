#ifndef ITERATIVE_POLICY_EVALUATION_H
#define ITERATIVE_POLICY_EVALUATION_H

#include "cubic_engine/rl/algorithms/dp/dp_algo_base.h"
#include "cubic_engine/rl/worlds/discrete_world.h"
#include "cubic_engine/rl/policies/discrete_policy_base.h"


#include <cmath>
#include <memory>

namespace cengine{
namespace rl{

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
                        env_t& env, std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy);

    ///
    /// \brief step
    ///
    virtual void step()override final;

    ///
    /// \brief policy
    /// \return
    ///
    const cengine::rl::policies::DiscretePolicyBase& policy()const{return  *policy_;}

    ///
    /// \brief policy
    /// \return
    ///
    cengine::rl::policies::DiscretePolicyBase& policy(){return  *policy_;}

    ///
    /// \brief policy_ptr
    /// \return
    ///
    std::shared_ptr<policies::DiscretePolicyBase> policy_ptr(){return  policy_;}

    ///
    ///
    ///
    void update_policy_ptr(std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> ptr){policy_ = ptr;}

protected:

    ///
    /// \brief policy_
    ///
    std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy_;

};

template<typename TimeStepTp>
IterativePolicyEval<TimeStepTp>::IterativePolicyEval(uint_t n_max_itrs, real_t tolerance, real_t gamma,
                                         env_t& env, std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy)
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
                auto prob = std::get<0>(dyn);
                auto next_state = std::get<1>(dyn);
                auto reward = std::get<2>(dyn);
                auto done = std::get<3>(dyn);

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
