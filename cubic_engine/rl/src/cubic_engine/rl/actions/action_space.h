#ifndef ACTION_SPACE_H
#define ACTION_SPACE_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <string>
#include <vector>

namespace cengine {
namespace rl {
namespace actions {

struct ActionSpace
{
    std::string type;
    std::vector<uint_t> shape;
};

}

}

}

#endif // ACTION_SPACE_H
