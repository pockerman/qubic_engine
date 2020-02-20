#ifndef KRYLOV_SOLVER_OUTPUT_H
#define KRYLOV_SOLVER_OUTPUT_H

#include "kernel/utilities/algorithm_info.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"

namespace kernel {
namespace numerics {

struct KrylovSolverResult: public AlgInfo
{
    /// \brief The type of the preconditioner the solver is using
    PreconditionerType precondioner_type;

    /// \brief The type of the solver
    KrylovSolverType solver_type;

    /// \brief Print the information about the performance of
    /// the algorithm on the given stream
    virtual std::ostream& print(std::ostream& out)const;

};

inline
std::ostream& operator<<(std::ostream& out, const KrylovSolverResult& info ){
    return info.print(out);
}

}

}

#endif // KRYLOV_SOLVER_OUTPUT_H
