#ifndef GRID_WORLD_H
#define GRID_WORLD_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/worlds/grid_world_state.h"
#include "kernel/base/kernel_consts.h"

#include <map>
#include <vector>
#include <stdexcept>

namespace cengine {
namespace rl {
namespace worlds {


template<typename RewardTp>
class GridWorld: public World<GridWorldAction, GridWorldState, typename RewardTp::value_t>
{
public:

    typedef RewardTp reward_t;
    typedef typename World<GridWorldAction, GridWorldState, typename RewardTp::value_t>::action_t action_t;
    typedef typename World<GridWorldAction, GridWorldState, typename RewardTp::value_t>::state_t state_t;
    typedef typename World<GridWorldAction, GridWorldState, typename RewardTp::value_t>::reward_value_t reward_value_t;

    /// \brief Constructor
    GridWorld();

    /// \brief Destructor
    ~GridWorld();

    /// \brief Returns the state
    virtual state_t& sense()override final{return  current_state_;}

    /// \brief Transition to a new state by
    /// performing the given action
    virtual void step(const action_t&)override final;

    /// \brief Restart the world. This means that
    /// the state of the world will be that of...
    void restart(const state_t& start, const state_t& goal);

    /// \brief Set the state of the world
    void set_state(const state_t& state){current_state_ = state;}

    /// \brief Returns the reward associated
    /// with the last state transition
    virtual reward_value_t reward()const override final{return r_;}

    /// \brief The size of the world.  Namely,
    /// the total number of different positions in
    /// the grid
    uint_t size()const{return states_.size();}

    /// \brief Set up the cells map
    void set_states(std::vector<state_t>&& states);

    /// \brief returns the current state of the world
    const state_t& get_current_state()const{return current_state_; }

    /// \brief Execute the aid-th action in the current state
    void execute_action(uint_t aid);

    /// \brief Returns the state with the given id
    const state_t get_state_by_id(uint_t id)const;

    /// \brief The number of states of the world
    uint_t n_states()const{return states_.size();}

    /// \brief Returns the i-th state
    state_t& get_state(uint_t s){return states_[s];}

    /// \brief Returns the i-th state
    const state_t& get_state(uint_t s)const{return states_[s];}

private:

    /// \brief The starting state
    state_t start_{0};

    /// \brief The goal state
    state_t goal_;

    /// \brief The current state the world is in
    state_t current_state_;

    /// \brief The object responsible for
    /// producing the reward that the agent
    /// should receive
    reward_t reward_;

    /// \brief The reward that the agent should recieve
    /// after executing an action
    reward_value_t r_;

    /// \brief A map that describes the possible state transitions
    /// from one state to another. This is simply a list of
    /// cell ids the agent can transition to
    std::vector<state_t> states_;

    /// \brief Flag indicating if the world
    /// has the current_state_ and goal_ state
    /// equal
    bool finished_;
};

template<typename RewardTp>
GridWorld<RewardTp>::GridWorld()
    :
    World<GridWorldAction, GridWorldState, typename RewardTp::value_t>(),
    start_(),
    goal_(),
    current_state_(),
    reward_(),
    r_(0.0),
    states_(),
    finished_(false)
{}

template<typename RewardTp>
GridWorld<RewardTp>::~GridWorld()
{}

template<typename RewardTp>
void
GridWorld<RewardTp>::step(const typename GridWorld<RewardTp>::action_t& action){

    if(states_.empty()){
        throw std::logic_error("Cell connectivity is not established");
    }

    if(current_state_ == goal_){
        r_ = reward_.goal_reward();
        finished_ = true;
    }
    else{

        /// for the current state
        /// find out the index of the cell
        /// that the agent can transition to
        auto* next_state = current_state_.execute_action(action);

        if(next_state == nullptr){
            // the agent just came out of the grid
            // so finishe the game
            finished_ = true;
        }

        r_ = reward_.get_reward(action, current_state_, *next_state);
        current_state_ = *next_state;
    }
}


template<typename RewardTp>
void
GridWorld<RewardTp>::set_states(std::vector<state_t>&& states){
    states_ = states;
}

template<typename RewardTp>
void
GridWorld<RewardTp>::restart(const typename GridWorld<RewardTp>::state_t& start,
                             const typename GridWorld<RewardTp>::state_t& goal){

    start_ = start;
    goal_ = goal;
    current_state_ = start_;
    r_ = 0.0;
    finished_ = false;

}

template<typename RewardTp>
void
GridWorld<RewardTp>::execute_action(uint_t aid){

}

template<typename RewardTp>
const typename GridWorld<RewardTp>::state_t
GridWorld<RewardTp>::get_state_by_id(uint_t id)const{

}

}

}

}
#endif // GRID_WORLD_H
