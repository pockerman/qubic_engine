#ifndef BOUNDARY_CONDITIONS_TYPE_H
#define BOUNDARY_CONDITIONS_TYPE_H

namespace kernel {
namespace numerics {

enum class BCType{DIRICHLET,  ZERO_DIRICHLET,
                  NEUMANN, ZERO_NEUMANN,
                  INVALID_BC_TYPE};

}

}

#endif // BOUNDARY_CONDITIONS_TYPE_H
