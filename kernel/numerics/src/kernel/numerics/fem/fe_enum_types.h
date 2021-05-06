#ifndef FE_ENUM_TYPES_H
#define FE_ENUM_TYPES_H

#include "kernel/base/config.h"

#ifdef USE_FEM

namespace kernel{
namespace numerics {
namespace fem {

///
/// \brief The FE_Type enum. Useful enumeration for describing
/// fe spaces
///
enum class FE_Type{FE_E2, FE_E3, FE_E4,
                   FE_Q4, FE_Q9,
                   FE_TRI, INVALID_FE_TYPE};

}
}
}

#endif
#endif // FE_ENUM_TYPES_H
