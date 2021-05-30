#ifndef FV_INTERPOLATION_TYPES_H
#define FV_INTERPOLATION_TYPES_H

#include "kernel/base/config.h"

#ifdef USE_FVM

namespace kernel {
namespace numerics {

enum class FVInterpolationType{UD, CD,
                               MIX_UD_CD,
                               LINEAR,
                               INVALID_INTERPOLATION_SCHEME};

}

}

#endif
#endif // FV_INTERPOLATION_TYPES_H
