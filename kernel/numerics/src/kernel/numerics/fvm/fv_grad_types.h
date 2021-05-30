#ifndef FV_GRAD_TYPES_H
#define FV_GRAD_TYPES_H

#include "kernel/base/config.h"

#ifdef USE_FVM

namespace kernel{
namespace numerics{

enum class FVGradType{GAUSS,
                      GAUSS_CORRECTED,
                      INVALID_TYPE};
}
}

#endif
#endif // FV_GRAD_TYPES_H
