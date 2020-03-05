#ifndef FV_GRAD_TYPES_H
#define FV_GRAD_TYPES_H

namespace kernel
{
namespace numerics
{

enum class FVGradType{GAUSS,
                      GAUSS_CORRECTED,
                      INVALID_TYPE};
}
}

#endif // FV_GRAD_TYPES_H
