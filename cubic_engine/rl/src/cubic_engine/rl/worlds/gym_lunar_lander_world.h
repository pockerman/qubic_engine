#ifndef GYM_LUNAR_LANDER_WORLD_H
#define GYM_LUNAR_LANDER_WORLD_H

#include "cubic_engine/rl/worlds/gym_world_base.h"

#include <msgpack.hpp>
#include <vector>
#include <array>

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
    /// \brief The ResetResponse struct
    ///
    struct ResetResponse;

    ///
    /// \brief The StepRequest
    ///
    struct StepRequest
    {
        std::vector<std::vector<real_t> > actions;
        bool render;
        MSGPACK_DEFINE_MAP(actions, render);
    };

    ///
    /// \brief StepResponse
    ///
    struct StepResponse;

    ///
    /// \brief InfoResponse
    ///
    struct InfoResponse;

    ///
    /// \brief make_step_request
    /// \return
    ///
    static std::unique_ptr<StepRequest> make_step_request();

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
    /// \brief step_from_request
    /// \param request
    /// \return
    ///
    std::tuple<std::vector<state_t>, std::vector<real_t>,
               std::vector<bool>, std::vector<std::any>> step_from_request(std::unique_ptr<StepRequest> request);

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

    ///
    /// \brief observation_space_shape
    ///
    std::vector<uint_t> observation_space_shape()const;

    ///
    /// \brief observation_space_shape
    ///
    std::vector<uint_t> action_space_shape()const;

    ///
    /// \brief action_space_type
    /// \return
    ///
    std::string action_space_type()const;

    ///
    /// \brief info
    ///
    InfoResponse info();

private:



};

}

}

}

#endif // GYM_ATARI_WORLD_H
