#ifndef POLICY_BASE_H
#define POLICY_BASE_H

#include "cubic_engine/rl/policies/policy_type.h"

namespace cengine {
namespace rl {
namespace policies {

class PolicyBase
{
public:

    ///
    ///
    ///
    virtual ~PolicyBase() = default;

    ///
    /// \brief type
    /// \return
    ///
    PolicyType type()const{return type_;}

protected:

    ///
    ///
    ///
    PolicyBase(PolicyType type);

    ///
    /// \brief type_
    ///
    PolicyType type_;
};

inline
PolicyBase::PolicyBase(PolicyType type)
    :
      type_(type)
{}

}
}
}

#endif // POLICY_BASE_H
