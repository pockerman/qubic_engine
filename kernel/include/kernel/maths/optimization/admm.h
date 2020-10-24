#ifndef ADMM_H
#define ADMM_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/solvers/solver_base.h"
#include "kernel/maths/direct_solvers/direct_solver_base.h"
#include "kernel/maths/solvers/solver_type.h"
#include "kernel/maths/optimization/quadratic_problem.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/base/config.h"
#include "kernel/base/kernel_consts.h"

#ifdef USE_TRILINOS
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_multivector.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#endif

#include <iostream>


namespace kernel{
namespace maths {
namespace opt{

template<typename MatrixTp, typename VectorTp>
struct ADMMConfig
{

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef MatrixTp matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef VectorTp vector_t;

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
    real_t alpha;

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
    solver_t* solver;

    ///
    /// \brief ADMMData Constructor
    ///
    ADMMConfig();

    ///
    /// \brief ADMMData Constructor
    ///
    ADMMConfig(solver_t& solver, real_t rho,
             real_t sigma, uint_t iterations, real_t tolerance);

};

template<typename MatrixTp, typename VectorTp>
ADMMConfig<MatrixTp, VectorTp>::ADMMConfig()
    :
    rho(0.5),
    sigma(0.5),
    alpha(0.5),
    max_n_iterations(0),
    tol(KernelConsts::tolerance()),
    solver(nullptr)
{}

template<typename MatrixTp, typename VectorTp>
ADMMConfig<MatrixTp, VectorTp>::ADMMConfig(typename ADMMConfig<MatrixTp, VectorTp>::solver_t& solver_,
                                       real_t rho_, real_t sigma_,
                                       uint_t iterations, real_t tolerance)
    :
    rho(rho_),
    sigma(sigma_),
    alpha(0.5),
    max_n_iterations(iterations),
    tol(tolerance),
    solver(&solver_)
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
    typedef VectorTp vector_t;

    ///
    /// \brief output_t The optimizer output
    ///
    typedef vector_t output_t;

    ///
    /// \brief config_t Configuration type
    ///
    typedef ADMMConfig<matrix_t, vector_t> config_t;

    ///
    /// \brief ADMM Constructor
    ///
    ADMM(const config_t& data);

    ///
    /// \brief solve
    ///
    void solve(QuadraticProblem<matrix_t, vector_t>& qp)const;

    ///
    /// \brief solve_direct
    ///
    void solve_direct(QuadraticProblem<matrix_t, vector_t>& qp)const;

    ///
    /// \brief solve_direct
    ///
    void solve_iterative(QuadraticProblem<matrix_t, vector_t>& qp)const;

private:

    ///
    /// \brief data_. Data used by the solver
    ///
    const ADMMConfig<matrix_t, vector_t>* data_;

    ///
    /// \brief update_direct_rhs_ Update the rhs vector
    ///
    void update_direct_rhs_(vector_t& rhs, const vector_t& y,
                            const QuadraticProblem<matrix_t, vector_t>& qp)const;

    ///
    /// \brief update_direct_matrix_ Build the matrix needed for direct solution
    ///
    void update_direct_matrix_(matrix_t& mat, const QuadraticProblem<matrix_t, vector_t>& qp)const;

    ///
    /// \brief project_vector_. Project z vector from the QP instance
    /// to min-max constraints
    ///
    void project_vector_(QuadraticProblem<matrix_t, vector_t>& qp,
                             const vector_t& ztilda, const vector_t& y)const;
};


}
}
}

#endif // ADMM_H
