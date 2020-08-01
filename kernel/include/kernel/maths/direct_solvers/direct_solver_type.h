#ifndef DIRECT_SOLVER_TYPE_H
#define DIRECT_SOLVER_TYPE_H

#include <string>

namespace kernel{
namespace maths {
namespace solvers {

enum class DirectSolverType{SUPER_LU, INVALID_TYPE};

///
/// \brief solver_type_to_string convert SolverType to std::string
///
std::string solver_type_to_string(DirectSolverType type);

}
}
}

#endif // DIRECT_SOLVER_TYPE_H
