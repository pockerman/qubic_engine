#ifndef ADMM_H
#define ADMM_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/solvers/solver_base.h"
#include "kernel/maths/solvers/solver_type.h"
#include "kernel/base/config.h"

#ifdef USE_TRILINOS
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_multivector.h"
#endif

#include <stdexcept>

namespace kernel{
namespace maths {
namespace opt{

template<typename MatrixTp, typename VectorTp>
struct ADMMData
{

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef MatrixTp matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef MatrixTp vector_t;

    ///
    /// \brief solver_t The type of the solver used by ADMM
    ///
    typedef kernel::maths::solvers::SolverBase<matrix_t, vector_t> solver_t;

    ///
    /// \brief rho_. The step size of the algorithm
    ///
    real_t rho;

    ///
    /// \brief sigma
    ///
    real_t sigma;

    ///
    /// \brief apha Relaxation factor
    ///
    real_t apha;

    ///
    /// \brief max_n_iterations. The maximum number of iterations
    ///
    uint_t max_n_iterations;

    ///
    /// \brief tol The tolerance used by the solver
    ///
    real_t tol;

    ///
    /// \brief solver. Reference to the solver used by ADMM
    ///
    solver_t& solver;


    ///
    /// \brief ADMMData Constructor
    ///
    ADMMData(solver_t& solver, real_t rho, real_t sigma, uint_t iterations, real_t tolerance);

    ///
    /// \brief ADMMData Constructor
    ///
    ADMMData(solver_t& solver, real_t rho, real_t sigma);

    ///
    /// \brief ADMMData Constructor
    ///
    ADMMData(solver_t& solver, real_t rho, real_t sigma, uint_t iterations);

};

template<typename MatrixTp, typename VectorTp>
ADMMData<MatrixTp, VectorTp>::ADMMData(typename ADMMData<MatrixTp, VectorTp>::solver_t& solver_,
                                       real_t rho_, real_t sigma_,
                                       uint_t iterations, real_t tolerance)
    :
    rho(rho_),
    sigma(sigma_),
    apha(0.5),
    max_n_iterations(iterations),
    tol(tolerance),
    solver(solver_)
{}

template<typename MatrixTp, typename VectorTp>
ADMMData<MatrixTp, VectorTp>::ADMMData(typename ADMMData<MatrixTp, VectorTp>::solver_t& solver_, real_t rho_, real_t sigma_)
    :
      ADMMData(solver_, rho_, sigma_, 1000, KernelConsts::tolerance())
{}

template<typename MatrixTp, typename VectorTp>
ADMMData<MatrixTp, VectorTp>::ADMMData(typename ADMMData<MatrixTp, VectorTp>::solver_t& solver,
                                       real_t rho, real_t sigma, uint_t iterations)
    :
    ADMMData(solver, rho, sigma, iterations, KernelConsts::tolerance())
{}



///
/// \brief The ADMM class. Implements the ADMM algorithm for solving the
/// convex quadratic problem
/// \f[ \text{minimize}~~ \frac{1}{2}x^TPx +q^Tx \f]
///
/// subject to
///
/// \f[ l \leq Ax \leq u \f]
///


template<typename MatrixTp, typename VectorTp>
class ADMM
{

public:

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef MatrixTp matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef MatrixTp vector_t;

    ///
    /// \brief ADMM Constructor
    ///
    ADMM(ADMMData<matrix_t, vector_t>& data);

#ifdef USE_TRILINOS
    ///
    /// \brief solve
    ///
    void solve(kernel::numerics::TrilinosEpetraMatrix& P,
               kernel::numerics::TrilinosEpetraMatrix& A);

    ///
    /// \brief solve_direct
    ///
    void solve_direct(kernel::numerics::TrilinosEpetraMatrix& P,
                      kernel::numerics::TrilinosEpetraMatrix& A);

    ///
    /// \brief solve_direct
    ///
    void solve_iterative(kernel::numerics::TrilinosEpetraMatrix& P,
                        kernel::numerics::TrilinosEpetraMatrix& A);
#endif

private:

    ADMMData<matrix_t, vector_t>& data_;
};

template<typename MatrixTp, typename VectorTp>
ADMM<MatrixTp, VectorTp>::ADMM(ADMMData<matrix_t, vector_t>& data)
    :
    data_(data)
{}

#ifdef USE_TRILINOS
template<>
void
ADMM<kernel::numerics::TrilinosEpetraMatrix,
     kernel::algebra::TrilinosEpetraMultiVector>::solve(kernel::numerics::TrilinosEpetraMatrix& P,
                                                        kernel::numerics::TrilinosEpetraMatrix& A){



    if(data_.solver.type() == kernel::maths::solvers::SolverType::DIRECT){
        solve_direct(P, A);
    }
    else if(data_.solver.type() == kernel::maths::solvers::SolverType::ITERATIVE){
        solve_iterative(P,A);
    }
    else{
        throw std::logic_error("Uniknown SolverType. SolverType should be DIRECT or ITERATIVE");
    }
}

template<>
void
ADMM<kernel::numerics::TrilinosEpetraMatrix,
     kernel::algebra::TrilinosEpetraMultiVector>::solve_direct(kernel::numerics::TrilinosEpetraMatrix& P,
                                                               kernel::numerics::TrilinosEpetraMatrix& A){

    // sol has two components x and v
    kernel::algebra::TrilinosEpetraMultiVector sol;
    kernel::algebra::TrilinosEpetraMultiVector rhs;

    //solver_ptr_->solve(A, sol, rhs);

    kernel::algebra::TrilinosEpetraMultiVector z;
    kernel::algebra::TrilinosEpetraMultiVector x;

    // the Lagrangian multipliers
    kernel::algebra::TrilinosEpetraMultiVector y;

    //z = z + (1.0/rho_)*()


}


template<>
void
ADMM<kernel::numerics::TrilinosEpetraMatrix,
     kernel::algebra::TrilinosEpetraMultiVector>::solve_iterative(kernel::numerics::TrilinosEpetraMatrix& P,
                                                                  kernel::numerics::TrilinosEpetraMatrix& A){

    // sol has two components x and v
    kernel::algebra::TrilinosEpetraMultiVector sol;
    kernel::algebra::TrilinosEpetraMultiVector rhs;

    //solver_ptr_->solve(A, sol, rhs);

    kernel::algebra::TrilinosEpetraMultiVector z;
    kernel::algebra::TrilinosEpetraMultiVector x;

    // the Lagrangian multipliers
    kernel::algebra::TrilinosEpetraMultiVector y;

    //z = z + (1.0/rho_)*()


}




#endif

}
}
}

#endif // ADMM_H
