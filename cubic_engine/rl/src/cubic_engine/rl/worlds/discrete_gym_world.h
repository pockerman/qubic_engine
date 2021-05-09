#ifndef DISCRETE_GYM_WORLD_H
#define DISCRETE_GYM_WORLD_H

#include "kernel/base/kernel_consts.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/discrete_world.h"
#include "cubic_engine/rl/dummy_environment_dynamics.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/gym_comm/requests.h"

#include <vector>
#include <any>
#include <string>
#include <memory>

namespace cengine {
namespace rl {
namespace worlds {


class DiscreteGymWorld
{
public:

    ///
    /// \brief action_t The action type for the world
    ///
    typedef uint_t action_t;

    ///
    /// \brief state_t The state type
    ///
    typedef std::vector<std::any> state_t;

    ///
    /// \brief reward_value_t The reward value type
    ///
    typedef real_t reward_value_t;

    ///
    /// \brief invalid_action Indicate an invalid action on the world
    /// Useful for uniform initialization when action is unknown
    ///
    static constexpr action_t INVALID_ACTION = kernel::KernelConsts::INVALID_SIZE_TYPE;

    ///
    /// \brief DiscreteGymWorld Constructor
    /// \param comm
    ///
    DiscreteGymWorld(gym::Communicator& comm);

    ///
    /// \brief Build the OpenAI Gym world
    ///
    virtual  void build(const std::string& world_name, bool reset=true, bool get_info_params=true);

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const;

    ///
    /// \brief n_actions. Max number of actions per state
    ///
    virtual uint_t n_actions()const;

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
    virtual std::tuple<state_t*, real_t, bool, std::any> step(const action_t&);

    ///
    /// \brief sample_action. Sample an action from
    /// the allowed action space of the world
    ///
    virtual const action_t sample_action()const;

    ///
    /// \brief returns the current state of the world
    ///
    const state_t& get_current_state()const{return current_state_; }

    ///
    /// \brief restart. Restart the world and
    /// return the starting state
    ///
    state_t* restart();

    ///
    /// \brief Returns true if the world is finished
    ///
    bool is_finished()const{return finished_;}

    ///
    /// \brief Returns the reward for the given state
    /// and the given actions
    ///
    reward_value_t get_reward(const state_t& state, const action_t& action)const;

private:

    ///
    /// \brief comm_ The object responsible for the
    /// communication with Gym OpenAI
    ///
    gym::Communicator* comm_;

    ///
    /// \brief current_state_. The current world state
    ///
    state_t current_state_;

    ///
    /// \brief is_built_ Flag indicating if the world has
    /// been built
    ///
    bool is_built_;

    ///
    /// \brief world_name_
    ///
    std::string world_name_;

    ///
    /// \brief Flag indicating if the world
    /// has the current_state_ and goal_ state equal
    ///
    bool finished_;


    ///
    /// \brief render_on_step_
    ///
    bool render_on_step_;

    ///
    /// \brief info_response_ Struc that holds basic informatin
    /// for the environement
    mutable std::unique_ptr<gym::InfoResponse> info_response_;


};




}

}

}

#endif // DISCRETE_GYM_WORLD_H
