#ifndef UNIFORM_DISCRETE_POLICY_H
#define UNIFORM_DISCRETE_POLICY_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/policies/discrete_policy_base.h"

#include <vector>
#include <utility>

namespace cengine{
namespace rl {
namespace policies {

///
/// \brief The UniformDiscretePolicy class
///
class UniformDiscretePolicy: public DiscretePolicyBase
{
public:

    ///
    /// \brief UniformDiscretePolicy
    ///
    UniformDiscretePolicy(uint_t n_states, uint_t n_actions);

    ///
    /// \brief UniformDiscretePolicy
    ///
    UniformDiscretePolicy(uint_t n_states, uint_t n_actions, real_t val);

    ///
    /// \brief operator []
    ///
    virtual std::vector<std::pair<uint_t, real_t>> operator[](uint_t sidx)const override final;

    ///
    /// \brief Update the policy for state with index sidx
    ///
    virtual void update(uint_t sidx, const std::vector<std::pair<uint_t, real_t>>& vals)override final;

    ///
    /// \brief equals
    ///
    virtual bool equals(const DiscretePolicyBase& other)const override final;

    ///
    /// \brief state_actions_values
    /// \return
    ///
    virtual std::vector<std::vector<std::pair<uint_t, real_t>>>& state_actions_values() override final{return state_actions_prob_;}

    ///
    /// \brief shape
    /// \return
    ///
    std::pair<uint_t, uint_t> shape()const{return {n_states_, n_actions_};}

    ///
    /// \brief make_copy. Make a copy of this
    ///
    virtual std::shared_ptr<DiscretePolicyBase> make_copy()const override final;

    ///
    /// \brief print
    /// \param out
    /// \return
    ///
    virtual std::ostream& print(std::ostream& out)const override final;

private:

    ///
    /// \brief n_states_
    ///
    uint_t n_states_;

    ///
    /// \brief n_actions_
    ///
    uint_t n_actions_;

    ///
    /// \brief val_
    ///
    real_t val_;

    ///
    /// \brief state_actions_prob_
    ///
    std::vector<std::vector<std::pair<uint_t, real_t>>> state_actions_prob_;

    ///
    /// \brief init_
    ///
    void init_();
};

}
}
}

#endif // UNIFORM_DISCRETE_POLICY_H
