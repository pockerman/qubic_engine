#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/rl/worlds/grid_world_state.h"

#include <random>
#include <stdexcept>
namespace cengine{
namespace rl {
namespace worlds {

GridWorldAction
GridWorldState::get_action_from_idx(uint_t i){

    switch(i){
    case 0:
        return GridWorldAction::SOUTH;
    case 1:
        return GridWorldAction::EAST;
    case 2:
        return GridWorldAction::NORTH;
    case 3:
        return GridWorldAction::WEST;
    default:
        throw std::logic_error("Invalid index for action mapping. Index " +
                               std::to_string(i) +
                               " not in [0,3]");

    }
}

GridWorldState::GridWorldState()
    :
      id_(kernel::KernelConsts::invalid_size_type()),
      state_transitions_()
{}

GridWorldState::GridWorldState(uint_t id)
    :
      id_(id),
      state_transitions_()
{}

GridWorldState::GridWorldState(const GridWorldState& other)
    :
      id_(other.id_),
      state_transitions_(other.state_transitions_)
{}

GridWorldState&
GridWorldState::operator=(const GridWorldState& other){

    if(&other == this){
        return *this;
    }

    this->id_ = other.id_;
    state_transitions_ = other.state_transitions_;
    return *this;
}

void
GridWorldState::set_transition(GridWorldAction action, GridWorldState* state){
    state_transitions_.insert_or_assign(action, state);
}

GridWorldState*
GridWorldState::execute_action(GridWorldAction action){
    return state_transitions_[action];
}

const GridWorldState*
GridWorldState::execute_action(GridWorldAction action)const{
    return state_transitions_.find(action)->second;
}

bool
GridWorldState::is_active_action(uint_t i)const{

    auto action = GridWorldState::get_action_from_idx(i);

    if(state_transitions_.find(action)->second){
        return true;
    }

    return false;
}

GridWorldAction
GridWorldState::get_action(uint_t a)const{
    return GridWorldState::get_action_from_idx(a);
}

GridWorldAction
GridWorldState::get_action_for_neighbor(const GridWorldState& neighbor)const{

    auto begin = state_transitions_.begin();
    auto end = state_transitions_.end();

    for(; begin != end; ++begin){

        if(begin->second != nullptr &&
                begin->second->get_id() == neighbor.get_id()){
            return begin->first;
        }
    }

   return GridWorldAction::INVALID_ACTION;

}

const std::vector<GridWorldAction>
GridWorldState::get_active_actions()const{

    std::vector<GridWorldAction> active_actions;
    active_actions.reserve(state_transitions_.size());

    auto itr = state_transitions_.begin();
    auto end = state_transitions_.end();

    for(; itr != end; ++itr){

        if(itr->second){
            active_actions.push_back(itr->first);
        }
    }

    return active_actions;
}

GridWorldAction
GridWorldState::get_random_active_action()const{

    auto active_actions = get_active_actions();

    ///Will be used to obtain a seed for the random number engine
    std::random_device rd;

    ///Standard mersenne_twister_engine seeded with rd()
    calculate: std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, active_actions.size()-1);

    auto idx = dis(rd);

    previous_active_action_choice_ = idx;

    return active_actions[idx];

}

std::vector<const GridWorldState*>
GridWorldState::get_states()const{

    std::vector<const GridWorldState*> states;
    states.reserve(state_transitions_.size());

    auto itr = state_transitions_.begin();
    auto end = state_transitions_.end();

    for(; itr != end; ++itr){
        states.push_back(itr->second);
    }

    return states;
}

bool
GridWorldState::has_action(GridWorldAction action)const{
    return state_transitions_.find(action) != state_transitions_.end();
}




}
}
}

#endif
