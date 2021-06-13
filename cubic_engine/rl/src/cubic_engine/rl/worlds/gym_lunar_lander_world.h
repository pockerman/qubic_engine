#ifndef GYM_LUNAR_LANDER_WORLD_H
#define GYM_LUNAR_LANDER_WORLD_H

#include "cubic_engine/rl/worlds/gym_world_base.h"

#include <vector>

namespace cengine {
namespace rl {
namespace gym {

///
/// forward declaration
///
class Communicator;
}


namespace worlds {


///
/// \brief The GymLunarLanderWorld class
///
class GymLunarLanderWorld: public GymWorldBase<std::vector<real_t>, uint_t>
{
public:

    ///
    /// \brief state_t
    ///
    typedef GymWorldBase<std::vector<real_t>, uint_t>::state_t state_t;

    ///
    /// \brief GymLunarLanderWorld
    ///
    GymLunarLanderWorld(const std::string& version, gym::Communicator& comm);

    ///
    /// \brief Transition to a new state by
    /// performing the given action. It returns a tuple
    /// with the following information
    /// arg1: An observation of the environment.
    /// arg2: Amount of reward achieved by the previous action.
    /// arg3: Flag indicating whether it’s time to reset the environment again.
    /// Most (but not all) tasks are divided up into well-defined episodes,
    /// and done being True indicates the episode has terminated.
    /// (For example, perhaps the pole tipped too far, or you lost your last life.)
    /// arg4: The type depends on the subclass overriding this function
    /// diagnostic information useful for debugging. It can sometimes be useful for
    /// learning (for example, it might contain the raw probabilities behind the environment’s last state change).
    /// However, official evaluations of your agent are not allowed to use this for learning.
    ///
    virtual std::tuple<state_t, real_t, bool, std::any> step(const action_t&) override;

    ///
    /// \brief restart. Restart the world and
    /// return the starting state
    ///
    virtual state_t reset()override;

    ///
    /// \brief Build the OpenAI Gym world
    ///
    virtual  void build(bool reset) override;

    ///
    /// \brief n_copies Returns the  number of copies of the environment
    ///
    virtual uint_t n_copies()const override {return 1;}

private:

    ///
    /// \brief The GymLunarLanderWorldResetResponse struct
    ///
    struct ResetResponse;

    ///
    /// \brief GymLunarLanderWorldStepResponse
    ///
    struct StepResponse;

    ///
    /// \brief GymLunarLanderWorldInfoResponse
    ///
    struct InfoResponse;


};

}

}

}

#endif // GYM_ATARI_WORLD_H
