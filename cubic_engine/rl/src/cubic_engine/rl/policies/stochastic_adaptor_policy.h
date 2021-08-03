#ifndef STOCHASTIC_ADAPTOR_POLICY_H
#define STOCHASTIC_ADAPTOR_POLICY_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/policies/policy_adaptor_base.h"

#include <memory>
namespace cengine {
namespace rl {
namespace policies {

class DiscretePolicyBase;

///
/// \brief The StochasticAdaptorPolicy class
///
class StochasticAdaptorPolicy: public DiscretePolicyAdaptorBase
{

public:

    ///
    /// \brief StochasticAdaptorPolicy
    /// \param policy
    ///
    StochasticAdaptorPolicy(uint_t state_space_size, uint_t action_space_size, std::shared_ptr<DiscretePolicyBase> policy);

    ///
    /// \brief Destructor
    ///
    ~StochasticAdaptorPolicy();

    ///
    /// \brief operator ()
    /// \param options
    /// \return
    ///
    virtual std::shared_ptr<DiscretePolicyBase> operator()(const std::map<std::string, std::any>& options);

private:

    ///
    /// \brief state_space_size_
    ///
    uint_t state_space_size_;

    ///
    /// \brief action_space_size_
    ///
    uint_t action_space_size_;

    ///
    /// \brief policy_
    ///
    std::shared_ptr<DiscretePolicyBase> policy_;
};
}

}

}

#endif // STOCHASTIC_ADAPTOR_POLICY_H
