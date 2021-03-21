#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#include <random>

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

GridWorldAction uniform_sample_grid_world_action(){

    // Will be used to obtain a seed for the random number engine
    std::random_device rd;

    // Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);
    auto idx = dis(rd);
    return static_cast<GridWorldAction>(idx);
}

}
}
}

#endif
