#ifndef ADMM_H
#define ADMM_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/solvers/solver_base.h"
#include "kernel/maths/direct_solvers/direct_solver_base.h"
#include "kernel/maths/solvers/solver_type.h"
#include "kernel/maths/optimization/quadratic_problem.h"
#include "kernel/base/config.h"

#ifdef USE_TRILINOS
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_multivector.h"
#include "kernel/maths/trilinos_epetra_vector.h"
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
    solver_t& solver;

    ///
    /// \brief ADMMData Constructor
    ///
    ADMMData(solver_t& solver, real_t rho,
             real_t sigma, uint_t iterations, real_t tolerance);

};

template<typename MatrixTp, typename VectorTp>
ADMMData<MatrixTp, VectorTp>::ADMMData(typename ADMMData<MatrixTp, VectorTp>::solver_t& solver_,
                                       real_t rho_, real_t sigma_,
                                       uint_t iterations, real_t tolerance)
    :
    rho(rho_),
    sigma(sigma_),
    alpha(0.5),
    max_n_iterations(iterations),
    tol(tolerance),
    solver(solver_)
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
    /// \brief ADMM Constructor
    ///
    ADMM(ADMMData<matrix_t, vector_t>& data);

    ///
    /// \brief solve
    ///
    void solve(QuadraticProblem<matrix_t, vector_t>& qproblem);

    ///
    /// \brief solve_direct
    ///
    void solve_direct(QuadraticProblem<matrix_t, vector_t>& qproblem);

    ///
    /// \brief solve_direct
    ///
    void solve_iterative(QuadraticProblem<matrix_t, vector_t>& qproblem);


private:

    ///
    /// \brief data_. Data used by the solver
    ///
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
     kernel::maths::algebra::TrilinosEpetraMultiVector>::solve_direct(QuadraticProblem<kernel::numerics::TrilinosEpetraMatrix,
                                                                           kernel::maths::algebra::TrilinosEpetraMultiVector>& qproblem){

    // sol has two components x and v
    kernel::numerics::TrilinosEpetraMatrix mat(qproblem.P.m() + qproblem.A.m(), 0);

    kernel::numerics::TrilinosEpetraMatrix At(qproblem.A.n(), 0);
    qproblem.A.compute_transpose(At);

    using kernel::maths::algebra::TrilinosEpetraMultiVector;

    TrilinosEpetraMultiVector sol;

    // the Lagrangian multipliers
    TrilinosEpetraMultiVector y;
    TrilinosEpetraMultiVector rhs(1, qproblem.x.size() + qproblem.A.m(), 0.0);

    for(uint_t i=0; i < qproblem.x.size(); ++i){
       rhs.set_entry(0, i, data_.sigma*qproblem.x.get(0, i) - qproblem.q.get(0, i));
    }

    for(uint_t i=0; i < qproblem.z.size(); ++i){
       rhs.set_entry(0, i + qproblem.x.size(),
                     qproblem.z.get(0, i) - (1.0/data_.rho)*y.get(0, i));
    }

    TrilinosEpetraMultiVector nu;

    TrilinosEpetraMultiVector z_tilda(1, qproblem.z.size(), 0.0);

    // establish the martix to solve

    // the first nxn entries are P + sigma I
    // the remaing nxm are A^T
    for(uint_t r=0; r<qproblem.P.m(); ++ r){

        auto p_row_entries = qproblem.P.get_row_entries(r);

        for(uint_t c=0; c<qproblem.P.n(); ++c){
            p_row_entries[c] += (c == r)? data_.sigma:0.0;
        }

        auto at_row_entries = At.get_row_entries(r);

        auto total_entries = p_row_entries;
        mat.set_row_entries(r, total_entries);
    }

    for(uint_t r = 0; r<qproblem.A.m(); ++r){
        auto a_row_entries = qproblem.A.get_row_entries(r);

        std::vector<real_t> unit_entries(qproblem.A.m(), 0.0);
        unit_entries[r] = -1.0/data_.rho;

        auto total_entries = a_row_entries;
        mat.set_row_entries(r + qproblem.P.m(), total_entries);

    }

    // form the rhs
    for(uint_t itr=0; itr<data_.max_n_iterations; ++itr){

        data_.solver.solve(mat, sol, rhs);

        // update the z tilda vector
        for(uint_t c=0; c<qproblem.z.size(); ++c){
            z_tilda.set_entry(0, c, (1.0/data_.rho)*(sol.get(1, c) - y.get(0, c)));
        }

        for(uint_t c=0; c<qproblem.x.size(); ++c){
            real_t x_previous = qproblem.x.get(0, c);
            qproblem.x.add_entry(0, c, data_.alpha*sol.get(0, c) +(1.0 - data_.alpha)*x_previous);
        }

        for(uint_t c=0; c<y.size(); ++c){
            //real_t y_previous = y.get(0, c);
            //real_t value = data_.rho*(data_.alpha*sol.get(0, c) +(1.0 - data_.alpha)*x_previous);
            //y.add_entry(0, c, );
        }
    }


}

template<>
void
ADMM<kernel::numerics::TrilinosEpetraMatrix,
     kernel::maths::algebra::TrilinosEpetraMultiVector>::solve_iterative(QuadraticProblem<kernel::numerics::TrilinosEpetraMatrix,
                                                              kernel::maths::algebra::TrilinosEpetraMultiVector>& qproblem){
    throw  std::logic_error("Not implemented");
}

template<>
void
ADMM<kernel::numerics::TrilinosEpetraMatrix,
     kernel::maths::algebra::TrilinosEpetraMultiVector>::solve(QuadraticProblem<kernel::numerics::TrilinosEpetraMatrix,
                                                                     kernel::maths::algebra::TrilinosEpetraMultiVector>& qproblem){



    if(data_.solver.type() == kernel::maths::solvers::SolverType::DIRECT){
        solve_direct(qproblem);
    }
    else if(data_.solver.type() == kernel::maths::solvers::SolverType::ITERATIVE){
        solve_iterative(qproblem);
    }
    else{
        throw std::logic_error("Uniknown SolverType. SolverType should be DIRECT or ITERATIVE");
    }
}

#endif

}
}
}

#endif // ADMM_H
