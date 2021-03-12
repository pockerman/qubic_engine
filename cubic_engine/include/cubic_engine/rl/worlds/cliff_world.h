#ifndef CLIFF_WORLD_H
#define CLIFF_WORLD_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"

#include "cubic_engine/rl/discrete_world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/worlds/grid_world_state.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/constant_environment_dynamics.h"

#include <stdexcept>
#include <tuple>

namespace cengine{
namespace rl{
namespace worlds {

namespace cliff_world_detail{

    ///
    /// \brief Inner Class that handles the rewards
    ///
    class CliffWorldRewardProducer
    {
    public:

        typedef real_t value_t;
        typedef GridWorldAction action_t;
        typedef GridWorldState state_t;

        ///
        /// \brief construcotr
        ///
        CliffWorldRewardProducer();

        ///
        /// \brief Returns the reward for the goal
        ///
        real_t goal_reward()const{return 0.0;}

        ///
        /// \brief Returns the reward for the action
        /// at  state s when going to state sprime
        ///
        real_t get_reward(const action_t& action,
                          const state_t& s,
                          const state_t& sprime)const;

        ///
        /// \brief Setup the rewards
        ///
        void setup_rewards();
    private:

        ///
        /// \brief The table that holds the rewards
        ///
        RewardTable<GridWorldAction, real_t> rewards_;
    };


    ///
    /// \brief The CliffWorldDynamics class. The class
    /// describing the dynamics
    ///
    /*class CliffWorldDynamics
    {
    public:

        ///
        /// \brief ConstantEnvironmentDynamics Constructor
        ///
        CliffWorldDynamics()=default;

        ///
        /// \brief ConstantEnvironmentDynamics Constructor
        ///
        CliffWorldDynamics(real_t val);

        ///
        /// \brief operator () Return the constant dynamics value
        ///
        template<typename StateTp, typename ActionTp>
        real_t operator()(const StateTp& state, const ActionTp& action)const{return 0.0;}

    private:

    };*/

} // cliff_world_detail

///
/// \brief CliffWorld class models the Environment
/// for the cliff-world problem. This is a standard un-discounted,
/// episodic task, with start and goal states, and the usual actions
/// causing movement up, down, right, and left.
/// Reward is -1 on all transitions except
/// those into the region marked Cliff.
/// Stepping into this region incurs a reward of
/// optimal path -100 and sends the agent instantly back to the start i.e. state zero
///
class CliffWorld final: public DiscreteWorld<GridWorldAction,
                                             GridWorldState,
                                             cliff_world_detail::CliffWorldRewardProducer,
                                             ConstantEnvironmentDynamics>
{

public:

    ///
    /// \brief action_t The action type
    ///
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState,
                                   cliff_world_detail::CliffWorldRewardProducer,
                                   ConstantEnvironmentDynamics>::action_t action_t;

    ///
    /// \brief state_t The state type
    ///
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState,
                                   cliff_world_detail::CliffWorldRewardProducer,
                                   ConstantEnvironmentDynamics>::state_t state_t;

    ///
    /// \brief reward_value_t The reward value type
    ///
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState,
                                   cliff_world_detail::CliffWorldRewardProducer,
                                   ConstantEnvironmentDynamics>::reward_value_t reward_value_t;

    ///
    /// \brief dynamics_t The dynamics objet
    ///
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState,
                                   cliff_world_detail::CliffWorldRewardProducer,
                                   ConstantEnvironmentDynamics>::dynamics_t dynamics_t;

    ///
    /// \brief Global invalid action assumed by the world.
    ///
    static const GridWorldAction invalid_action = GridWorldAction::INVALID_ACTION;

    ///
    /// \brief Constructor
    ///
    CliffWorld();

    ///
    /// \brief Destructor
    ///
    ~CliffWorld();

    ///
    /// \brief Returns the current state
    ///
    virtual const state_t& sense()override final{return  *this->current_state_;}

    ///
    /// \brief sample_action. Sample an action from
    /// the allowed action space of the world. The Uniform
    /// distribution is used for sampling
    ///
    virtual const action_t sample_action()const override final;

    ///
    /// \brief Transition to a new state by
    /// performing the given action
    ///
    virtual std::tuple<state_t*, real_t, bool, std::any> step(const action_t&)override final;

    ///
    /// \brief Returns the reward associated
    /// with the last state transition
    ///
    reward_value_t reward()const{return r_;}

    ///
    /// \brief Returns the current state of the world
    ///
    const state_t& get_current_state()const{return *current_state_; }

    ///
    /// \brief Execute the aid-th action in the current state
    ///
    void execute_action(action_t aid);

    ///
    /// \brief Create the world
    ///
    void build();

    ///
    /// \brief save_world_as_csv. Save the world in csv format in the
    /// file specified by the given filename
    ///
    virtual void save_world_as_csv(const std::string& filename)const override final{}

private:

    ///
    /// \brief The reward that the agent should recieve
    /// after executing an action
    ///
    reward_value_t r_;

};

inline
void
CliffWorld::execute_action(CliffWorld::action_t aid){
    current_state_ = const_cast<state_t*>(current_state_)->execute_action(aid);
}


}
}
}

#endif // USE_RL
#endif // CLIFF_WORLD_H
