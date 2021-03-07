#include "cubic_engine/rl/worlds/grid_world_action_space.h"

namespace cengine {
namespace rl {
namespace worlds {

std::string to_string(GridWorldAction action){

    switch(action){
    case GridWorldAction::EAST:
        return "EAST";
    case GridWorldAction::WEST:
        return "WEST";
    case GridWorldAction::NORTH:
        return "NORTH";
    case GridWorldAction::SOUTH:
        return "SOUTH";
    }

    return "INVALID_ACTION";
}
}
}
}

