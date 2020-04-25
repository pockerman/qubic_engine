#ifndef CLIFF_WORLD_H
#define CLIFF_WORLD_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/world.h"
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include "cubic_engine/rl/worlds/grid_world_state.h"

#include  <stdexcept>

namespace cengine{
namespace rl{
namespace worlds {


struct CliffWorldReward
{
    /// \brief The reward received when  reaching the goal
    real_t  goal_reward() const {return    0.;}

    /// \brief The reward received when doing one step
    real_t step_reward() const {return   -1;}

    /// \brief The reward received when falling off the cliff
    real_t fall_reward() const {return -100.;}

    /// \brief The reward received
    real_t bump_reward() const {return  step_reward();}
};


/// \brief CliffWorld class models the Environment
/// for the cliff-world problem
class CliffWorld: public World<GridWorldAction, GridWorldState, real_t>
{

public:

    typedef real_t reward_t;
    typedef typename World<GridWorldAction, GridWorldState, real_t>::action_t action_t;
    typedef typename World<GridWorldAction, GridWorldState, real_t>::state_t state_t;
    typedef typename World<GridWorldAction, GridWorldState, real_t>::reward_value_t reward_value_t;

    /// \brief Global invalid action assumed by the
    /// world. Useful for initialization.
    static const GridWorldAction invalid_action = GridWorldAction::INVALID_ACTION;

    /// \brief Constructor
    CliffWorld();

    /// \brief Destructor
    ~CliffWorld();

    /// \brief Returns the state
    virtual const state_t& sense()override final{return  *current_state_;}

    /// \brief Transition to a new state by
    /// performing the given action
    virtual void step(const action_t&)override final;

    /// \brief Restart the world. This means that
    /// the state of the world will be that of...
    void restart(const state_t& start, const state_t& goal);

    /// \brief Set the state of the world
    void set_state(const state_t& state){current_state_ = &state;}

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


private:

    /// \brief The starting state
    const state_t* start_;

    /// \brief The goal state
    const state_t* goal_;

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

inline
void
CliffWorld::execute_action(CliffWorld::action_t aid){
    current_state_ = const_cast<state_t*>(current_state_)->execute_action(aid);
}

inline
void
CliffWorld::set_states(std::vector<CliffWorld::state_t>&& states){
    states_ = states;
}

inline
void
CliffWorld::restart(const typename CliffWorld::state_t& start,
                    const typename CliffWorld::state_t& goal){

    start_ = &start;
    goal_ = &goal;
    current_state_ = &get_state(start_->get_id());
    r_ = 0.0;
    finished_ = false;

}


inline
const CliffWorld::state_t&
CliffWorld::get_state(uint_t id)const{

    if(id >= states_.size()){
        throw std::logic_error("Invalid state id: "+
                               std::to_string(id) +
                               " not in [0, "+
                               std::to_string(states_.size()));
    }

    return states_[id];
}

inline
CliffWorld::state_t&
CliffWorld::get_state(uint_t id){

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

#endif // CLIFF_WORLD_H
