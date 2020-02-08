#ifndef KRYLOV_SOLVER_DATA_H
#define KRYLOV_SOLVER_DATA_H

#include "kernel/base/types.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"

namespace kernel{
namespace numerics {

struct KrylovSolverData{

    uint_t n_iterations;
    real_t tolerance;
    PreconditionerType precondioner_type;
    KrylovSolverType solver_type;
};

}
}

#endif // KRYLOV_SOLVER_DATA_H
