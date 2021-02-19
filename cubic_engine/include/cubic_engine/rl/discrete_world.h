#ifndef DISCRETE_WORLD_H
#define DISCRETE_WORLD_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/world.h"

#include <vector>
#include <algorithm>

namespace cengine {
namespace rl {

///
/// \brief The DiscreteWorld class. Class for modeling discrete
/// worlds in reinforcement learning
///
template<typename ActionTp, typename StateTp, typename RewardTp>
class DiscreteWorld: public World<ActionTp, StateTp, RewardTp>
{
public:

    typedef typename World<ActionTp, StateTp, RewardTp>::reward_t reward_t;
    typedef typename World<ActionTp, StateTp, typename RewardTp::value_t>::action_t action_t;
    typedef typename World<ActionTp, StateTp, typename RewardTp::value_t>::state_t state_t;
    typedef typename World<ActionTp, StateTp, typename RewardTp::value_t>::reward_value_t reward_value_t;

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const{return states_.size();}

    ///
    /// \brief Transition to a new state by
    /// performing the given action
    ///
    virtual void step(const action_t&)=0;

    ///
    /// \brief Returns true if the given state is a goal state
    ///
    bool is_goal_state(const state_t& state)const;

    ///
    /// \brief Returns the i-th state
    ///
    state_t& get_state(uint_t s);

    ///
    /// \brief Returns the i-th state
    ///
    const state_t& get_state(uint_t s)const;

    ///
    /// \brief returns the current state of the world
    ///
    const state_t& get_current_state()const{return *current_state_; }

    ///
    /// \brief Restart the world. This means that
    /// the state of the world will be that of start
    ///
    void restart(const state_t& start, const state_t& goal);

    ///
    /// \brief Restart the world with a list of goals
    ///
    void restart(const state_t& start, std::vector<state_t*>&& goal);

    ///
    /// \brief Restart the world but keep the goals
    ///
    void restart(const state_t& state);

    ///
    /// \brief Set up the cells map
    ///
    void set_states(std::vector<state_t>&& states){states_ = states;}

    ///
    /// \brief Returns true if the world is finished
    ///
    bool is_finished()const{return finished_;}

    ///
    /// \brief Append another goal
    ///
    void append_goal(const state_t& goal){goals_.push_back(&goal);}

    ///
    /// \brief Returns the reward for the given state
    /// and the given actions
    ///
    reward_value_t get_reward(const state_t& state, const action_t& action)const{
        return this->reward_.get_reward(action, state);
    }

protected:

    ///
    /// \brief DiscreteWorld. Constructor
    ///
    DiscreteWorld();

    ///
    /// \brief The current state
    ///
    state_t* current_state_;

    ///
    /// \brief The goal states. More than one goal
    /// states are allowed
    ///
    std::vector<const state_t*> goals_;

    ///
    /// \brief A map that describes the possible state transitions
    /// from one state to another. This is simply a list of
    /// cell ids the agent can transition to
    ///
    std::vector<state_t> states_;

    ///
    /// \brief Flag indicating if the world
    /// has the current_state_ and goal_ state equal
    ///
    bool finished_;
};

template<typename ActionTp, typename StateTp, typename RewardTp>
DiscreteWorld<ActionTp, StateTp, RewardTp>::DiscreteWorld()
    :
    World<ActionTp, StateTp, RewardTp>(),
    current_state_(nullptr),
    goals_(),
    states_(),
    finished_(false)
{}

template<typename ActionTp, typename StateTp, typename RewardTp>
bool
DiscreteWorld<ActionTp, StateTp, RewardTp>::is_goal_state(const state_t& state)const{

    return std::find_if(goals_.begin(),
                        goals_.end(),
                        [&](const auto* ptr){return ptr->get_id() == state.get_id();}) != goals_.end();
}

template<typename ActionTp, typename StateTp, typename RewardTp>
const typename DiscreteWorld<ActionTp, StateTp, RewardTp>::state_t&
DiscreteWorld<ActionTp, StateTp, RewardTp>::get_state(uint_t id)const{

    if(id >= states_.size()){
        throw std::logic_error("Invalid state id: "+
                               std::to_string(id) +
                               " not in [0, "+
                               std::to_string(states_.size()));
    }

    return states_[id];
}

template<typename ActionTp, typename StateTp, typename RewardTp>
typename DiscreteWorld<ActionTp, StateTp, RewardTp>::state_t&
DiscreteWorld<ActionTp, StateTp, RewardTp>::get_state(uint_t id){

    if(id >= states_.size()){
        throw std::logic_error("Invalid state id: "+
                               std::to_string(id) +
                               " not in [0, "+
                               std::to_string(states_.size()));
    }

    return states_[id];
}


template<typename ActionTp, typename StateTp, typename RewardTp>
void
DiscreteWorld<ActionTp, StateTp, RewardTp>::restart(const state_t& start,
                             const state_t& goal){

    current_state_ = &start;
    goals_ = std::vector<const state_t*>();
    goals_.push_back( &goal);
    finished_ = false;

}

template<typename ActionTp, typename StateTp, typename RewardTp>
void
DiscreteWorld<ActionTp, StateTp, RewardTp>::restart(const state_t& start,
                             std::vector<state_t*>&& goals){

    current_state_ = &start;
    goals_ = std::vector<const state_t*>();
    goals_ = goals;
    finished_ = false;
}

template<typename ActionTp, typename StateTp, typename RewardTp>
void
DiscreteWorld<ActionTp, StateTp, RewardTp>::restart(const state_t& start){
    current_state_ = &start;
    finished_ = false;
}

}

}

#endif
#endif // DISCRETE_WORLD_H
