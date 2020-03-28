#ifndef BLAZE_SOLUTION_POLICY_H
#define BLAZE_SOLUTION_POLICY_H

#include "kernel/base/types.h"

namespace kernel {
namespace numerics {

/// forward declarations
class BlazeKrylovSolver;
struct KrylovSolverResult;

/// \brief A solution policy should
/// expose the type of the matrix and vector
/// data structures as well as the solver type
struct BlazeSolutionPolicy
{
    typedef SparseMatrix<real_t> matrix_t;
    typedef DynVec<real_t> vector_t;
    typedef BlazeKrylovSolver solver_t;
    typedef KrylovSolverResult solver_output_t;
};


}
}

#endif // BLAZE_SOLUTION_POLICY_H
