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
class GridWorld: public World<GridWorldAction,
                              GridWorldState,
                              typename RewardTp::value_t>
{
public:

    typedef RewardTp reward_t;
    typedef typename World<GridWorldAction, GridWorldState, typename RewardTp::value_t>::action_t action_t;
    typedef typename World<GridWorldAction, GridWorldState, typename RewardTp::value_t>::state_t state_t;
    typedef typename World<GridWorldAction, GridWorldState, typename RewardTp::value_t>::reward_value_t reward_value_t;

    /// \brief Global invalid action assumed by the
    /// world. Useful for initialization.
    static const GridWorldAction invalid_action = GridWorldAction::INVALID_ACTION;

    /// \brief Constructor
    GridWorld();

    /// \brief Destructor
    ~GridWorld();

    /// \brief Returns the state
    virtual const state_t& sense()override final{return  *current_state_;}

    /// \brief Transition to a new state by
    /// performing the given action
    virtual void step(const action_t&)override final;

    /// \brief Restart the world. This means that
    /// the state of the world will be that of start
    void restart(const state_t& start, const state_t& goal);

    /// \brief Restart the world with a list of goals
    void restart(const state_t& start, std::vector<state_t*>&& goal);

    /// \brief Restart the world but keep the goals
    void restart(const state_t& state);

    /// \brief Set the state of the world
    void set_state(const state_t& state){current_state_ = &state;}

    /// \brief Append another goal
    void append_goal(const state_t& goal){goals_.push_back(&goal);}

    /// \brief Returns the reward associated
    /// with the last state transition
    virtual reward_value_t reward()const override final{return r_;}

    /// \brief Returns the reward for the given state
    /// and the given actions
    reward_value_t get_reward(const state_t& state, const action_t& action)const{
        return reward_.get_reward(action, state);
    }

    /// \brief The size of the world.  Namely,
    /// the total number of different positions in
    /// the grid
    uint_t size()const{return states_.size();}

    /// \brief Set up the cells map
    void set_states(std::vector<state_t>&& states);

    /// \brief returns the current state of the world
    const state_t& get_current_state()const{return *current_state_; }

    /// \brief Execute the aid-th action in the current state
    void execute_action(action_t aid);

    /// \brief The number of states of the world
    uint_t n_states()const{return states_.size();}

    /// \brief Returns the i-th state
    state_t& get_state(uint_t s);

    /// \brief Returns the i-th state
    const state_t& get_state(uint_t s)const;

    /// \brief Returns true if the world is finished
    bool is_finished()const{return finished_;}

    /// \brief Returns true if the given state is a goal state
    bool is_goal_state(const state_t& state)const;

private:

    /// \brief The starting state
    const state_t* start_;

    /// \brief The goal states. More than one goal
    /// states are allowed
    std::vector<const state_t*> goals_;

    /// \brief The current state the world is in
    const state_t* current_state_;

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
    start_(nullptr),
    goals_(),
    current_state_(nullptr),
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

    if(current_state_ == nullptr ){
       throw std::logic_error("Null current state pointer");
    }

    if(goals_.empty()){
       throw std::logic_error("Empty goals list");
    }

    if(is_goal_state(*current_state_)){
        r_ = reward_.goal_reward();
        finished_ = true;
    }
    else{

        /// for the current state
        /// find out the index of the cell
        /// that the agent can transition to
        auto* next_state = const_cast<state_t*>(current_state_)->execute_action(action);

        if(next_state == nullptr){
            /// the agent just came out of the grid
            /// so finish the game
            finished_ = true;

            /// what is the return for this?
            r_ = reward_.get_reward(action, *current_state_);
        }
        else{
            r_ = reward_.get_reward(action, *current_state_, *next_state);
            current_state_ = next_state;
        }
    }
}


template<typename RewardTp>
void
GridWorld<RewardTp>::set_states(std::vector<state_t>&& states){
    states_ = states;
}

template<typename RewardTp>
bool
GridWorld<RewardTp>::is_goal_state(const state_t& state)const{

    return std::find_if(goals_.begin(),
                        goals_.end(),
                        [&](const auto* ptr){return ptr->get_id() == state.get_id();}) != goals_.end();
}

template<typename RewardTp>
void
GridWorld<RewardTp>::restart(const state_t& start,
                             const state_t& goal){

    start_ = &start;
    goals_ = std::vector<const state_t*>();
    goals_.push_back( &goal);
    current_state_ = &get_state(start_->get_id());
    r_ = 0.0;
    finished_ = false;

}

template<typename RewardTp>
void
GridWorld<RewardTp>::restart(const state_t& start,
                             std::vector<state_t*>&& goals){

    start_ = &start;
    goals_ = std::vector<const state_t*>();
    goals_ = goals;
    current_state_ = &get_state(start_->get_id());
    r_ = 0.0;
    finished_ = false;

}

template<typename RewardTp>
void
GridWorld<RewardTp>::restart(const state_t& start){

    start_ = &start;
    current_state_ = &get_state(start_->get_id());
    r_ = 0.0;
    finished_ = false;
}

template<typename RewardTp>
void
GridWorld<RewardTp>::execute_action(typename GridWorld<RewardTp>::action_t aid){

    current_state_ = const_cast<state_t*>(current_state_)->execute_action(aid);
}

template<typename RewardTp>
const typename GridWorld<RewardTp>::state_t&
GridWorld<RewardTp>::get_state(uint_t id)const{

    if(id >= states_.size()){
        throw std::logic_error("Invalid state id: "+
                               std::to_string(id) +
                               " not in [0, "+
                               std::to_string(states_.size()));
    }

    return states_[id];
}

template<typename RewardTp>
typename GridWorld<RewardTp>::state_t&
GridWorld<RewardTp>::get_state(uint_t id){

    if(id >= states_.size()){
        throw std::logic_error("Invalid state id: "+
                               std::to_string(id) +
                               " not in [0, "+
                               std::to_string(states_.size()));
    }

    return states_[id];
}

}

}

}
#endif // GRID_WORLD_H
