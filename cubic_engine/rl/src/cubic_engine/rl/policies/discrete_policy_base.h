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
    virtual ~DiscretePolicyBase();

    ///
    /// \brief operator []
    ///
    virtual std::vector<std::pair<uint_t, real_t>> operator[](uint_t sidx)const = 0;

    ///
    /// \brief Update the policy for state with index sidx
    ///
    virtual void update(uint_t sidx, const std::vector<std::pair<uint_t, real_t>>& vals)=0;

    ///
    /// \brief state_actions_values
    ///
    virtual std::vector<std::vector<std::pair<uint_t, real_t>>>& state_actions_values()=0;

    ///
    /// \brief equals
    ///
    virtual bool equals(const DiscretePolicyBase& other)const = 0;

    ///
    /// \brief make_copy. Make a copy of this
    ///
    virtual std::shared_ptr<DiscretePolicyBase> make_copy()const = 0;

protected:

    ///
    /// \brief DiscretePolicyBase
    /// \param n_states
    /// \param n_actions
    ///
    DiscretePolicyBase(PolicyType type);

};


inline
bool operator==(const DiscretePolicyBase& p1, const DiscretePolicyBase& p2){
    return p1.equals(p2);
}

inline
bool operator !=(const DiscretePolicyBase& p1, const DiscretePolicyBase& p2){
    return !(p1 == p2);
}

inline
bool operator==(const std::shared_ptr<DiscretePolicyBase>& p1, const std::shared_ptr<DiscretePolicyBase>& p2){
    return p1->equals(*p2);
}

inline
bool operator !=(const std::shared_ptr<DiscretePolicyBase>& p1, const std::shared_ptr<DiscretePolicyBase>& p2){
    return !(p1 == p2);
}

}

}

}

#endif // DISCRETE_POLICY_BASE_H
