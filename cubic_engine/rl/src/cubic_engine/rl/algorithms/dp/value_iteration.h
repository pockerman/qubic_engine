#ifndef VALUE_ITERATION_H
#define VALUE_ITERATION_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/algorithms/dp/dp_algo_base.h"
#include "cubic_engine/rl/algorithms/dp/policy_improvement.h"
#include "cubic_engine/rl/algorithms/utils.h"
#include "cubic_engine/rl/policies/policy_adaptor_base.h"
#include "cubic_engine/rl/policies/discrete_policy_base.h"

#include <memory>
#include <cmath>

namespace cengine{
namespace rl {
namespace algos {
namespace dp {

///
/// \brief ValueIteration class
///
template<typename TimeStepTp>
class ValueIteration: public DPAlgoBase<TimeStepTp>
{
public:

    ///
    /// \brief env_t
    ///
    typedef typename DPAlgoBase<TimeStepTp>::env_t env_t;

    ///
    /// \brief ValueIteration
    ///
    ValueIteration(uint_t n_max_iterations, real_t tolerance,
                   env_t& env, real_t gamma, std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy,
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
    /// \brief policy_ptr
    /// \return
    ///
    std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy_ptr(){return  policy_;}

    ///
    /// \brief update_policy_ptr
    /// \param ptr
    ///
    void update_policy_ptr(std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> ptr){policy_ = ptr;}

private:

    ///
    /// \brief policy_
    ///
    std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy_;

    ///
    /// \brief policy_imp_
    ///
    PolicyImprovement<TimeStepTp> policy_imp_;

};

template<typename TimeStepTp>
ValueIteration<TimeStepTp>::ValueIteration(uint_t n_max_iterations, real_t tolerance,
                                           env_t& env, real_t gamma, std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy,
                                           std::shared_ptr<cengine::rl::policies::DiscretePolicyAdaptorBase> policy_adaptor)
    :
   DPAlgoBase<TimeStepTp>(n_max_iterations, tolerance, gamma, env),
   policy_(policy),
   policy_imp_(1, gamma, DynVec<real_t>(), env, policy, policy_adaptor)
{}


template<typename TimeStepTp>
void
ValueIteration<TimeStepTp>::actions_before_training_iterations(){
    this->DPAlgoBase<TimeStepTp>::actions_before_training_iterations();
    policy_imp_.actions_before_training_iterations();
}

template<typename TimeStepTp>
void
ValueIteration<TimeStepTp>::step(){


    auto delta = 0.0;
    for(uint_t s=0; s< this->env_ref_().n_states(); ++s){

        auto v = this->value_func()[s];
        this->value_func()[s] = blaze::max(state_actions_from_v(this->env_ref_(), this->value_func(), s, this->gamma()));
        delta = std::max(delta, std::fabs(this->value_func()[s] - v));
    }

    this->iter_controller_().residual = delta;

    policy_imp_.value_func() = this->value_func();
    policy_imp_.step();
    policy_ = policy_imp_.policy().make_copy();
}

}
}
}
}

#endif // VALUE_ITERATION_H
