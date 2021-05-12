#ifndef DISCRETE_GYM_WORLD_WITH_DYNAMICS_H
#define DISCRETE_GYM_WORLD_WITH_DYNAMICS_H

#include "cubic_engine/rl/worlds/discrete_gym_world.h"
#include "cubic_engine/rl/gym_comm/communicator.h"

#include <vector>
#include <tuple>
#include <string>

namespace cengine{
namespace rl {
namespace worlds{

class DiscreteGymWorldWithDynamics: public DiscreteGymWorld<uint_t>
{

public:

    typedef DiscreteGymWorld::action_t action_t;
    typedef DiscreteGymWorld::state_t state_t;
    typedef DiscreteGymWorld::reward_value_t reward_value_t;


    ///
    /// \brief DiscreteGymWorldWithDynamics
    ///
    DiscreteGymWorldWithDynamics(gym::Communicator& comm);

    ///
    /// \brief get_dynamics
    /// \param state
    /// \param action
    /// \return
    ///
    std::vector<std::tuple<real_t, state_t, reward_value_t, bool>> get_dynamics(const state_t& state, const action_t& action);

};
}

}
}

#endif // DISCRETE_GYM_WORLD_WITH_DYNAMICS_H
