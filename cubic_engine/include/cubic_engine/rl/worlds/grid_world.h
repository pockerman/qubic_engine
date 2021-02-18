#ifndef GRID_WORLD_H
#define GRID_WORLD_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/worlds/grid_world_state.h"
#include "cubic_engine/rl/discrete_world.h"
#include "kernel/base/kernel_consts.h"

#include <map>
#include <vector>
#include <stdexcept>

namespace cengine {
namespace rl {
namespace worlds {

///
/// \brief GridWorld class. Models the grid world environment
///
template<typename RewardTp>
class GridWorld final: public DiscreteWorld<GridWorldAction,
                                            GridWorldState,
                                            typename RewardTp::value_t>
{
public:

    typedef RewardTp reward_t;
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState, typename RewardTp::value_t>::action_t action_t;
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState, typename RewardTp::value_t>::state_t state_t;
    typedef typename DiscreteWorld<GridWorldAction, GridWorldState, typename RewardTp::value_t>::reward_value_t reward_value_t;

    /// \brief Global invalid action assumed by the
    /// world. Useful for initialization.
    static const GridWorldAction invalid_action = GridWorldAction::INVALID_ACTION;

    ///
    /// \brief Constructor
    ///
    GridWorld();

    ///
    /// \brief Destructor
    ///
    ~GridWorld();

    ///
    /// \brief Returns the state
    ///
    virtual const state_t& sense()override final{return  *this->current_state_;}

    ///
    /// \brief Transition to a new state by
    /// performing the given action
    ///
    virtual void step(const action_t&)override final;


    /// \brief Set the state of the world
    void set_state(const state_t& state){this->current_state_ = &state;}


    /// \brief Returns the reward associated
    /// with the last state transition
    virtual reward_value_t reward()const override final{return r_;}

    /// \brief Returns the reward for the given state
    /// and the given actions
    reward_value_t get_reward(const state_t& state, const action_t& action)const{
        return reward_.get_reward(action, state);
    }


    /// \brief Execute the aid-th action in the current state
    void execute_action(action_t aid);

private:

    ///
    /// \brief The object responsible for
    /// producing the reward that the agent
    /// should receive
    ///
    reward_t reward_;

    ///
    /// \brief The reward that the agent should recieve
    /// after executing an action
    ///
    reward_value_t r_;


};

template<typename RewardTp>
GridWorld<RewardTp>::GridWorld()
    :
    DiscreteWorld<GridWorldAction, GridWorldState, typename RewardTp::value_t>(),
    reward_(),
    r_(0.0)
{}

template<typename RewardTp>
GridWorld<RewardTp>::~GridWorld()
{}

template<typename RewardTp>
void
GridWorld<RewardTp>::step(const typename GridWorld<RewardTp>::action_t& action){

    if(this->states_.empty()){
        throw std::logic_error("Cell connectivity is not established");
    }

    if(this->current_state_ == nullptr ){
       throw std::logic_error("Null current state pointer");
    }

    if(this->goals_.empty()){
       throw std::logic_error("Empty goals list");
    }

    if(this->is_goal_state(*this->current_state_)){
        r_ = reward_.goal_reward();
        this->finished_ = true;
    }
    else{

        // for the current state
        // find out the index of the cell
        // that the agent can transition to
        auto* next_state = const_cast<state_t*>(this->current_state_)->execute_action(action);

        if(next_state == nullptr){

            // the agent just came out of the grid
            // so finish the game
            this->finished_ = true;

            // what is the return for this?
            r_ = reward_.get_reward(action, *this->current_state_);
        }
        else{
            r_ = reward_.get_reward(action, *this->current_state_, *next_state);
            this->current_state_ = next_state;
        }
    }
}




template<typename RewardTp>
void
GridWorld<RewardTp>::execute_action(typename GridWorld<RewardTp>::action_t aid){

    this->current_state_ = const_cast<state_t*>(current_state_)->execute_action(aid);
}



}

}

}
#endif // GRID_WORLD_H
