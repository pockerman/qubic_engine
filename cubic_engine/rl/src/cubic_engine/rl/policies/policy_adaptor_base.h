#ifndef POLICY_ADAPTOR_BASE_H
#define POLICY_ADAPTOR_BASE_H

#include <any>
#include <map>
#include <string>
#include <memory>

namespace cengine {
namespace rl {
namespace policies {

class PolicyBase;

///
/// \brief The PolicyAdaptorBase class
///
class PolicyAdaptorBase
{
public:


    ///
    /// \brief operator ()
    /// \param options_ptr
    /// \return
    ///
    virtual std::shared_ptr<PolicyBase> operator()(const std::map<std::string, std::any>& options)=0;

protected:

    ///
    /// \brief PolicyAdaptorBase
    /// \param policy
    ///
    PolicyAdaptorBase(std::shared_ptr<PolicyBase> policy);

    ///
    /// \brief policy_
    ///
    std::shared_ptr<PolicyBase> policy_;



};

inline
PolicyAdaptorBase::PolicyAdaptorBase(std::shared_ptr<PolicyBase> policy)
    :
      policy_(policy)
{}

}
}
}

#endif // POLICY_ADAPTOR_BASE_H
