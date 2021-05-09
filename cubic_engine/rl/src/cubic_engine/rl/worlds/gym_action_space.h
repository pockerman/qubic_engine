#ifndef GYM_ACTION_SPACE_H
#define GYM_ACTION_SPACE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <string>
#include <vector>

namespace cengine {
namespace rl{
namespace worlds{

///
/// \brief The GymActionSpace struct. Action space
/// for Gym environments
///
struct GymActionSpace
{
    std::string type;
    std::vector<int64_t> shape;
};
}

}

}

#endif // GYM_ACTION_SPACE_H
