#ifndef POLICY_BASE_H
#define POLICY_BASE_H

#include "cubic_engine/rl/policies/policy_type.h"
#include <ostream>

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

    ///
    /// \brief print
    ///
    virtual std::ostream& print(std::ostream& out)const = 0;

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


inline
std::ostream& operator<<(std::ostream& out, const PolicyBase& p){
    return p.print(out);
}

}
}
}

#endif // POLICY_BASE_H
