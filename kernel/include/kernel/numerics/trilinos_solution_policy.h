#ifndef TRILINOS_SOLUTION_POLICY_H
#define TRILINOS_SOLUTION_POLICY_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

namespace kernel {
namespace numerics {

/// forward declarations
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;
class TrilinosKrylovSolver;
struct KrylovSolverResult;

/// \brief A solution policy should
/// expose the type of the matrix and vector
/// data structures as well as the solver type
struct TrilinosSolutionPolicy
{
    typedef TrilinosEpetraMatrix matrix_t;
    typedef TrilinosEpetraVector vector_t;
    typedef TrilinosKrylovSolver solver_t;
    typedef KrylovSolverResult solver_output_t;
};

}

}

#endif
#endif // TRILINOS_SOLUTION_POLICY_H
