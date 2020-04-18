#include "cubic_engine/rl/worlds/grid_world_state.h"
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


}

}
}
