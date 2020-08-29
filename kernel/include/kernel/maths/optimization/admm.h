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

#ifdef USE_TRILINOS
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_multivector.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#endif

#include <iostream>
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
    ADMMData<matrix_t, vector_t>& data_;

    ///
    /// \brief update_direct_rhs_ Update the rhs vector
    ///
    void update_direct_rhs_(vector_t& rhs, const vector_t& y,
                            const QuadraticProblem<matrix_t, vector_t>& qp)const;

    ///
    /// \brief update_direct_matrix_ Build the matrix needed for direct solution
    ///
    void update_direct_matrix_(matrix_t& mat, const QuadraticProblem<matrix_t, vector_t>& qp)const;
};

template<typename MatrixTp, typename VectorTp>
ADMM<MatrixTp, VectorTp>::ADMM(ADMMData<matrix_t, vector_t>& data)
    :
    data_(data)
{}

template<>
void
ADMM<DynMat<real_t>, DynVec<real_t>>::update_direct_rhs_(DynVec<real_t>& rhs,
                                                         const DynVec<real_t>& y,
                                                         const QuadraticProblem<DynMat<real_t>, DynVec<real_t>>& qp)const{

    for(uint_t i=0; i < qp.x.size(); ++i){
       rhs[i] = data_.sigma*qp.x[i] - qp.q[i];
    }

    for(uint_t i=0; i < qp.z.size(); ++i){
       rhs[i + qp.x.size()] = qp.z[i] - (1.0/data_.rho)*y[i];
    }

    // form the rhs
    /*for(uint_t r=0; r<rhs.size(); ++r){

        if(r < qp.P.rows()){
            rhs[r] = data_.sigma*qp.x[r] - qp.q[r];
        }
        else{
           rhs[r] = qp.z[r - rhs.size()] - (1.0/data_.rho)*y[r - rhs.size()];
        }
     }*/

}

template<>
void
ADMM<DynMat<real_t>, DynVec<real_t>>::update_direct_matrix_(DynMat<real_t>& mat,
                                                            const QuadraticProblem<DynMat<real_t>, DynVec<real_t>>& qp)const{

    // establish the martix to solve
    DynMat<real_t> At = trans(qp.A);

    // the first nxn entries are P + sigma I
    // the remaing nxm are A^T
    for(uint_t r=0; r<qp.P.rows(); ++ r){

        auto p_row_entries = get_row(qp.P, r);

        for(uint_t c=0; c<qp.P.columns(); ++c){
            p_row_entries[c] += (c == r)? data_.sigma:0.0;
        }

        auto at_row_entries = get_row(At, r);

        for(uint c=0; c<mat.columns(); ++c){

            if(c < qp.P.rows()){
                mat(r, c) = p_row_entries[c];
            }
            else{
               mat(r, c) = at_row_entries[c - qp.P.rows()];
            }
        }
    }


    std::vector<real_t> unit_entries(qp.A.rows(), -1.0/data_.rho);

    // set the remaining m-rows
    for(uint_t r = 0; r<qp.A.rows(); ++r){

        auto a_row_entries = get_row(qp.A, r);

        for(uint c=0; c<mat.columns(); ++c){

            if(c < qp.A.columns()){
                mat(r + qp.P.rows(), c) = a_row_entries[c];
            }
            else{
                mat(r + qp.P.rows(), c) = unit_entries[c - qp.P.rows()];
            }
        }
    }
}

template<>
void
ADMM<DynMat<real_t>, DynVec<real_t>>::solve_direct(QuadraticProblem<DynMat<real_t>, DynVec<real_t>>& qp)const{


    if(qp.P.rows() != qp.x.size()){
        throw std::logic_error("Number of rows not equal to state vector size");
    }

    if(qp.q.size() != qp.x.size()){
        throw std::logic_error("Unequal state and q vectors sizes");
    }

    if(qp.A.rows() != qp.z.size()){
        throw std::logic_error("Number of rows not equal to z vector size");
    }

    if(qp.P.rows() != qp.A.columns()){
       throw std::logic_error("Number of rows not equal to number of columns");
    }

    DynMat<real_t> mat(qp.P.rows() + qp.A.rows(), qp.P.rows() + qp.A.rows(), 0.0);
    DynVec<real_t> sol(qp.x.size() + qp.A.rows(), 0.0);

    // the Lagrangian multipliers
    DynVec<real_t> y(qp.z.size(), 0.0);
    DynVec<real_t> rhs(qp.x.size() + qp.A.rows(), 0.0);

    // update the rhs
    update_direct_rhs_(rhs, y, qp);

    std::cout<<"Rhs: "<<rhs<<std::endl;

    // update the direct matrix
    update_direct_matrix_(mat, qp);

    std::cout<<"Matrix: "<<mat<<std::endl;
    std::cout<<"Solution: "<<sol<<std::endl;

    /*for(uint_t i=0; i < qp.x.size(); ++i){
       rhs[i] = (0, i, data_.sigma*qp.x[i] - qp.q[i]);
    }

    for(uint_t i=0; i < qp.z.size(); ++i){
       rhs[i + qp.x.size()] = qp.z[i] - (1.0/data_.rho)*y[i];
    }*/

    DynVec<real_t> z_tilda(qp.z.size(), 0.0);

    data_.solver.solve(mat, sol, rhs);
    //for(uint_t itr=0; itr<data_.max_n_iterations; ++itr){



        // update the z tilda vector
        /*for(uint_t c=0; c<qp.z.size(); ++c){
            z_tilda[c] = (1.0/data_.rho)*(sol[c] - y[c]);
        }

        for(uint_t c=0; c<qp.x.size(); ++c){
            real_t x_previous = qp.x[c];
            qp.x[c] += data_.alpha*sol[c] +(1.0 - data_.alpha)*x_previous;
        }

        for(uint_t c=0; c<y.size(); ++c){
            //real_t y_previous = y.get(0, c);
            //real_t value = data_.rho*(data_.alpha*sol.get(0, c) +(1.0 - data_.alpha)*x_previous);
            //y.add_entry(0, c, );
        }*/

        // update rhs
        //update_direct_rhs_(rhs, y, qp);
    //}
}

template<>
void
ADMM<DynMat<real_t>, DynVec<real_t>>::solve_iterative(QuadraticProblem<DynMat<real_t>, DynVec<real_t>>& qp)const{
    throw  std::logic_error("Not implemented");
}


#ifdef USE_TRILINOS

template<>
void
ADMM<kernel::numerics::TrilinosEpetraMatrix,
     kernel::maths::algebra::TrilinosEpetraMultiVector>::solve_direct(QuadraticProblem<kernel::numerics::TrilinosEpetraMatrix,
                                                                           kernel::maths::algebra::TrilinosEpetraMultiVector>& qp)const{

    // sol has two components x and v
    kernel::numerics::TrilinosEpetraMatrix mat(qp.P.m() + qp.A.m(), 0);

    kernel::numerics::TrilinosEpetraMatrix At(qp.A.n(), 0);
    qp.A.compute_transpose(At);

    using kernel::maths::algebra::TrilinosEpetraMultiVector;

    TrilinosEpetraMultiVector sol;

    // the Lagrangian multipliers
    TrilinosEpetraMultiVector y;
    TrilinosEpetraMultiVector rhs(1, qp.x.size() + qp.A.m(), 0.0);

    for(uint_t i=0; i < qp.x.size(); ++i){
       rhs.set_entry(0, i, data_.sigma*qp.x.get(0, i) - qp.q.get(0, i));
    }

    for(uint_t i=0; i < qp.z.size(); ++i){
       rhs.set_entry(0, i + qp.x.size(),
                     qp.z.get(0, i) - (1.0/data_.rho)*y.get(0, i));
    }

    TrilinosEpetraMultiVector nu;

    TrilinosEpetraMultiVector z_tilda(1, qp.z.size(), 0.0);

    // establish the martix to solve

    // the first nxn entries are P + sigma I
    // the remaing nxm are A^T
    for(uint_t r=0; r<qp.P.m(); ++ r){

        auto p_row_entries = qp.P.get_row_entries(r);

        for(uint_t c=0; c<qp.P.n(); ++c){
            p_row_entries[c] += (c == r)? data_.sigma:0.0;
        }

        auto at_row_entries = At.get_row_entries(r);

        auto total_entries = p_row_entries;
        mat.set_row_entries(r, total_entries);
    }

    for(uint_t r = 0; r<qp.A.m(); ++r){
        auto a_row_entries = qp.A.get_row_entries(r);

        std::vector<real_t> unit_entries(qp.A.m(), 0.0);
        unit_entries[r] = -1.0/data_.rho;

        auto total_entries = a_row_entries;
        mat.set_row_entries(r + qp.P.m(), total_entries);

    }

    // form the rhs
    for(uint_t itr=0; itr<data_.max_n_iterations; ++itr){

        data_.solver.solve(mat, sol, rhs);

        // update the z tilda vector
        for(uint_t c=0; c<qp.z.size(); ++c){
            z_tilda.set_entry(0, c, (1.0/data_.rho)*(sol.get(1, c) - y.get(0, c)));
        }

        for(uint_t c=0; c<qp.x.size(); ++c){
            real_t x_previous = qp.x.get(0, c);
            qp.x.add_entry(0, c, data_.alpha*sol.get(0, c) +(1.0 - data_.alpha)*x_previous);
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
                                                              kernel::maths::algebra::TrilinosEpetraMultiVector>& qp)const{
    throw  std::logic_error("Not implemented");
}

#endif

template<typename MatrixTp, typename VectorTp>
void
ADMM<MatrixTp, VectorTp>::solve(QuadraticProblem<MatrixTp, VectorTp>& qp)const{

    if(data_.solver.type() == kernel::maths::solvers::SolverType::DIRECT){
        solve_direct(qp);
    }
    else if(data_.solver.type() == kernel::maths::solvers::SolverType::ITERATIVE){
        solve_iterative(qp);
    }
    else{
        throw std::logic_error("Uniknown SolverType. SolverType should be DIRECT or ITERATIVE");
    }
}

}
}
}

#endif // ADMM_H
