#ifndef DISCRETE_WORLD_H
#define DISCRETE_WORLD_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/worlds/world_base.h"

#include <vector>
#include <tuple>
#include <string>

namespace cengine{
namespace rl{
namespace envs {

class TimeStep;

///
/// \brief The DiscreteWorldBase class
///
template<typename TimeStepTp>
class DiscreteWorldBase: public WorldBase<uint_t, uint_t, TimeStepTp>
{
public:

    ///
    /// \brief state_t
    ///
    typedef typename WorldBase<uint_t, uint_t, TimeStepTp>::state_t state_t;

    ///
    /// \brief action_t
    ///
    typedef typename WorldBase<uint_t, uint_t, TimeStepTp>::action_t action_t;

    ///
    /// \brief time_step_t
    ///
    typedef typename WorldBase<uint_t, uint_t, TimeStepTp>::time_step_t time_step_t;

    ///
    ///
    ///
    virtual ~DiscreteWorldBase() = default;

    ///
    /// \brief n_actions
    ///
    virtual uint_t n_actions()const = 0;

    ///
    /// \brief n_actions
    ///
    virtual uint_t n_states()const = 0;

    ///
    /// \brief transition_dynamics
    ///
    virtual std::vector<std::tuple<real_t, uint_t, real_t, bool>> transition_dynamics(uint_t s, uint_t aidx)const = 0;

protected:

    ///
    /// \brief DiscreteWorldBase
    /// \param name
    ///
    DiscreteWorldBase(std::string name);

};

template<typename TimeStepTp>
DiscreteWorldBase<TimeStepTp>::DiscreteWorldBase(std::string name)
    :
      WorldBase<uint_t, uint_t, TimeStepTp>(name)
{}

}
}
}

#endif // DISCRETE_WORLD_H
