#include "kernel/maths/direct_solvers/amesos2_solver.h"

#ifdef USE_TRILINOS

#include "kernel/maths/solvers/solver_package_type.h"

namespace kernel {
namespace maths {
namespace solvers {

Amesos2Direct::Amesos2Direct(DirectSolverType dstype)
    :
      DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                       kernel::numerics::TrilinosEpetraVector>(SolverPackageType::TRI_AMESOS2),
      dstype_(dstype)
{}

void
Amesos2Direct::solve(const Amesos2Direct::matrix_t& A, Amesos2Direct::vector_t& x, const Amesos2Direct::vector_t& b ){

}

}
}
}

#endif
