#ifndef POLICY_ADAPTOR_BASE_H
#define POLICY_ADAPTOR_BASE_H

#include "cubic_engine/rl/policies/discrete_policy_base.h"

#include <any>
#include <map>
#include <string>
#include <memory>

namespace cengine {
namespace rl {
namespace policies {


//class DiscretePolicyBase;

///
/// \brief The PolicyAdaptorBase class
///
class DiscretePolicyAdaptorBase
{
public:

    ///
    ///
    ///
    virtual ~DiscretePolicyAdaptorBase() = default;

    ///
    /// \brief operator ()
    /// \param options_ptr
    /// \return
    ///
    virtual std::shared_ptr<DiscretePolicyBase> operator()(const std::map<std::string, std::any>& options)=0;

protected:

    ///
    /// \brief PolicyAdaptorBase
    /// \param policy
    ///
    DiscretePolicyAdaptorBase() = default;



};

}
}
}

#endif // POLICY_ADAPTOR_BASE_H
