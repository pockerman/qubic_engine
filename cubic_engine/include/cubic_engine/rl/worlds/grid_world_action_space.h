#ifndef GRID_WORLD_ACTION_SPACE_H
#define GRID_WORLD_ACTION_SPACE_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include <string>
namespace cengine {
namespace rl {
namespace worlds {

///
/// \brief Enumeration describing the action
/// space for grid-based world
///
enum class GridWorldAction: uint_t {NORTH=0, SOUTH=1,
                                    EAST=2, WEST=3,
                                    INVALID_ACTION=4};

std::string to_string(GridWorldAction action);

}
}
}

#endif
#endif // GRID_WORLD_ACTION_SPACE_H
