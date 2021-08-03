#ifndef POLICY_IMPROVEMENT_H
#define POLICY_IMPROVEMENT_H

#include "cubic_engine/rl/algorithms/dp/dp_algo_base.h"
#include "cubic_engine/rl/algorithms/utils.h"
#include "cubic_engine/rl/policies/policy_adaptor_base.h"
#include "cubic_engine/rl/policies/discrete_policy_base.h"

#include <memory>
#include <any>
#include <map>
#include <string>

namespace cengine{
namespace rl {


namespace algos {
namespace dp {

///
/// \brief The PolicyImprovement class
///
template<typename TimeStepTp>
class PolicyImprovement: public DPAlgoBase<TimeStepTp>
{
public:

    ///
    /// \brief env_t
    ///
    typedef typename DPAlgoBase<TimeStepTp>::env_t env_t;

    ///
    /// \brief IterativePolicyEval
    ///
    PolicyImprovement(uint_t n_max_itrs,  real_t gamma, const DynVec<real_t>& val_func,
                      env_t& env, std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy,
                      std::shared_ptr<cengine::rl::policies::DiscretePolicyAdaptorBase> policy_adaptor);

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
    std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy_ptr(){return  policy_;}

    ///
    /// \brief update_policy_ptr
    /// \param ptr
    ///
    void update_policy_ptr(std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> ptr){policy_ = ptr;}


protected:

    ///
    /// \brief policy_
    ///
    std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy_;

    ///
    /// \brief policy_adaptor_
    ///
    std::shared_ptr<cengine::rl::policies::DiscretePolicyAdaptorBase> policy_adaptor_;

};

template<typename TimeStepTp>
PolicyImprovement<TimeStepTp>::PolicyImprovement(uint_t n_max_itrs,  real_t gamma, const DynVec<real_t>& val_func,
                                                 env_t& env, std::shared_ptr<cengine::rl::policies::DiscretePolicyBase> policy,
                                                 std::shared_ptr<cengine::rl::policies::DiscretePolicyAdaptorBase> policy_adaptor)
    :
      DPAlgoBase<TimeStepTp>(n_max_itrs, 1.0e-4, gamma, env),
      policy_(policy),
      policy_adaptor_(policy_adaptor)
{
    this->value_func() = val_func;
}

template<typename TimeStepTp>
void
PolicyImprovement<TimeStepTp>::step(){

    std::map<std::string, std::any> options;
    for(uint_t s=0; s<this->env_ref_().n_states(); ++s){

        auto state_actions = state_actions_from_v(this->env_ref_(), this->value_func(),
                                                  this->gamma(), s);

        options.insert_or_assign("state", s);
        options.insert_or_assign("state_actions", std::any(state_actions));
        this->policy_ = this->policy_adaptor_->operator()(options);

    }

    this->iter_controller_().update_residual(1.0e-5);

}


}

}

}
}

#endif // POLICY_IMPROVEMENT_H
