#ifndef TRILINOS_SOLUTION_POLICY_H
#define TRILINOS_SOLUTION_POLICY_H

namespace kernel {
namespace numerics {

/// forward declarations
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;
class TrilinosKrylovSolver;

/// \brief A solution policy should
/// expose the type of the matrix and vector
/// data structures
struct TrilinosSolutionPolicy
{
    typedef TrilinosEpetraMatrix matrix_t;
    typedef TrilinosEpetraVector vector_t;
    typedef TrilinosKrylovSolver solver_t;
};

}

}

#endif // TRILINOS_SOLUTION_POLICY_H
