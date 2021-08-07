#ifndef KRYLOV_SOLVER_OUTPUT_H
#define KRYLOV_SOLVER_OUTPUT_H

#include "kernel/utilities/iterative_algorithm_result.h"
#include "kernel/numerics/krylov_solvers/preconditioner_type.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_type.h"

namespace kernel {
namespace numerics {

struct KrylovSolverResult
{
    ///
    /// \brief The type of the preconditioner the solver is using
    ///
    PreconditionerType precondioner_type;

    ///
    /// \brief The type of the solver
    ///
    KrylovSolverType solver_type;

    /// \brief Print the information about the performance of
    /// the algorithm on the given stream
    std::ostream& print(std::ostream& out)const;

private:

    IterativeAlgorithmResult result_;

};

inline
std::ostream& operator<<(std::ostream& out, const KrylovSolverResult& info ){
    return info.print(out);
}

}
}

#endif // KRYLOV_SOLVER_OUTPUT_H
