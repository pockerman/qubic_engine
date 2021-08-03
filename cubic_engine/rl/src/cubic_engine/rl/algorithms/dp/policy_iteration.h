#ifndef POLICY_ITERATION_H
#define POLICY_ITERATION_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/algorithms/dp/dp_algo_base.h"
#include "cubic_engine/rl/algorithms/dp/iterative_policy_evaluation.h"
#include "cubic_engine/rl/algorithms/dp/policy_improvement.h"
#include "cubic_engine/rl/policies/policy_adaptor_base.h"
#include "cubic_engine/rl/policies/discrete_policy_base.h"

//#include <iostream>
#include <memory>

namespace cengine{
namespace rl{
namespace algos {
namespace dp{

template<typename TimeStepTp>
class PolicyIteration: public DPAlgoBase<TimeStepTp>
{
public:

    ///
    /// \brief env_t
    ///
    typedef typename DPAlgoBase<TimeStepTp>::env_t env_t;

    ///
    /// \brief PolicyIteration
    ///
    PolicyIteration(uint_t n_max_iterations, real_t tolerance, env_t& env,
                    real_t gamma, uint_t n_policy_eval_steps,
                    std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy,
                    std::shared_ptr<cengine::rl::policies::DiscretePolicyAdaptorBase> policy_adaptor);

    ///
    /// \brief step
    ///
    virtual void step() override final;

    ///
    /// \brief actions_before_training_iterations. Execute any actions the
    /// algorithm needs before starting the iterations
    ///
    virtual void actions_before_training_iterations() override final;

    ///
    /// \brief actions_after_training_iterations. Actions to execute after
    /// the training iterations have finisehd
    ///
    virtual void actions_after_training_iterations()override final{this->value_func() = policy_eval_.value_func();}

private:

    ///
    /// \brief policy_eval_
    ///
    IterativePolicyEval<TimeStepTp> policy_eval_;


    ///
    /// \brief policy_imp_
    ///
    PolicyImprovement<TimeStepTp> policy_imp_;

};

template<typename TimeStepTp>
PolicyIteration<TimeStepTp>::PolicyIteration(uint_t n_max_iterations, real_t tolerance, env_t& env,
                                             real_t gamma, uint_t n_policy_eval_steps,
                                             std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy,
                                             std::shared_ptr<cengine::rl::policies::DiscretePolicyAdaptorBase> policy_adaptor)
    :
    DPAlgoBase<TimeStepTp>(n_max_iterations, tolerance, gamma, env),
    policy_eval_(n_policy_eval_steps, tolerance, gamma, env, policy),
    policy_imp_(1, gamma, DynVec<real_t>(), env, policy, policy_adaptor)
{}


template<typename TimeStepTp>
void
PolicyIteration<TimeStepTp>::actions_before_training_iterations(){

    this->DPAlgoBase<TimeStepTp>::actions_before_training_iterations();
    policy_eval_.actions_before_training_iterations();
    policy_imp_.actions_before_training_iterations();
}

template<typename TimeStepTp>
void
PolicyIteration<TimeStepTp>::step(){

    // make a copy of the policy already obtained
    auto old_policy = policy_eval_.policy().make_copy();

    // evaluate the policy
    policy_eval_.train();

    // update the value function to
    // improve for
    policy_imp_.value_func() = policy_eval_.value_func();

    // improve the policy
    policy_imp_.train();

    auto new_policy = policy_imp_.policy_ptr();

    if(*old_policy == *new_policy){
        this->iter_controller_().update_residual(this->iter_controller_().get_exit_tolerance()*std::pow(10,-1));
        return;
    }


    policy_eval_.update_policy_ptr(new_policy);
    policy_eval_.reset();
    policy_imp_.reset();
}

}
}
}
}

#endif // POLICY_ITERATION_H
