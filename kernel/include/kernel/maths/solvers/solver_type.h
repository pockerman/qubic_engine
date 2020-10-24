#ifndef SOLVER_TYPE_H
#define SOLVER_TYPE_H

#include <string>

namespace kernel {
namespace maths{
namespace solvers{

///
/// \brief The SolverType enum. Enumeration of the two major solver types
/// kernellib supports
///
enum class SolverType{DIRECT, ITERATIVE, INVALID_TYPE};

///
/// \brief solver_type_to_string convert SolverType to std::string
///
std::string solver_type_to_string(SolverType type);
}
}

}

#endif // SOLVER_TYPE_H
