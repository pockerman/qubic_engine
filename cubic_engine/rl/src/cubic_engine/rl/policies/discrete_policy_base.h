#ifndef DISCRETE_POLICY_BASE_H
#define DISCRETE_POLICY_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/policies/policy_base.h"
#include "cubic_engine/rl/policies/policy_type.h"

#include <vector>
#include <utility>

namespace cengine {
namespace rl {
namespace policies {

///
/// \brief The DiscretePolicyBase class
///
class DiscretePolicyBase: public PolicyBase
{
public:

    ///
    /// \brief Destructor
    ///
    virtual ~DiscretePolicyBase() = default;

    ///
    /// \brief operator []
    ///
    virtual std::vector<std::pair<uint_t, real_t>> operator[](uint_t sidx)const = 0;

    ///
    /// \brief equals
    ///
    virtual bool equals(const DiscretePolicyBase& other)const = 0;

protected:

    ///
    /// \brief DiscretePolicyBase
    /// \param n_states
    /// \param n_actions
    ///
    DiscretePolicyBase(PolicyType type);

};

inline
DiscretePolicyBase::DiscretePolicyBase(PolicyType type)
    :
      PolicyBase(type)
{}

inline
bool operator==(const DiscretePolicyBase& p1, const DiscretePolicyBase& p2){
    return p1.equals(p2);
}

inline
bool operator !=(const DiscretePolicyBase& p1, const DiscretePolicyBase& p2){
    return !(p1 == p2);
}

}

}

}

#endif // DISCRETE_POLICY_BASE_H
