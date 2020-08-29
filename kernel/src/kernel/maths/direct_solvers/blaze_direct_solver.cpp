#include "kernel/maths/direct_solvers/blaze_direct_solver.h"
#include "kernel/maths/solvers/solver_package_type.h"

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
BlazeDirectSolver::solve(BlazeDirectSolver::matrix_t& A,
                         BlazeDirectSolver::vector_t& x, BlazeDirectSolver::vector_t& b ){

    if(A.columns() != x.size()){
        throw std::logic_error("Invalid Matrix-Vector size");
    }

    if(A.rows() != b.size()){
        throw std::logic_error("Invalid rhs size");
    }

    std::cout<<A<<std::endl;
    std::cout<<x<<std::endl;
    std::cout<<b<<std::endl;
    //auto rslt = A*x;


    // main solve method from Blaze
    // TODO: How to specify LU decomposition?
    solve( A, x, b );

}

}
}
}
