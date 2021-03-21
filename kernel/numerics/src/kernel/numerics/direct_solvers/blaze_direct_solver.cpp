#include "kernel/numerics/direct_solvers/blaze_direct_solver.h"
#include "kernel/numerics/solvers/solver_package_type.h"

#include <stdexcept>
#include <iostream>

namespace kernel{
namespace maths {
namespace solvers {

BlazeDirectSolver::BlazeDirectSolver(const BlazeDirectSolverConfig& config)
    :
    DirectSolverBase<DynMat<real_t>, DynVec<real_t>>(SolverPackageType::BLAZE),
    config_(config)
{}

void
BlazeDirectSolver::solve(const BlazeDirectSolver::matrix_t& A,
                         BlazeDirectSolver::vector_t& x,
                         const BlazeDirectSolver::vector_t& b )const{

    if(A.columns() != x.size()){
        throw std::logic_error("Invalid Matrix-Vector size");
    }

    if(A.rows() != b.size()){
        throw std::logic_error("Invalid rhs size");
    }

    // main solve method from Blaze
    // TODO: How to specify LU decomposition?
    blaze::solve( A, x, b );

}

}
}
}
