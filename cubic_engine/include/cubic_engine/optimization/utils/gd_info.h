#ifndef GD_INFO_H
#define GD_INFO_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/base/algorithm_info.h"

namespace cengine
{

struct GDInfo: public kernel::AlgInfo
{

    real_t learning_rate;

};

}

#endif // GD_INFO_H
