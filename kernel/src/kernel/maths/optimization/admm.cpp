#include "kernel/maths/optimization/admm.h"
#include <stdexcept>

namespace kernel{
namespace maths {
namespace opt{

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
ADMM<DynMat<real_t>, DynVec<real_t>>::project_vector_(QuadraticProblem<DynMat<real_t>, DynVec<real_t>>& qp,
                                                      const DynVec<real_t>& ztilda, const DynVec<real_t>& y)const{

    auto rho_inv = 1.0/data_.rho;
    DynVec<real_t> tmp = data_.alpha*ztilda + (1.0 - data_.alpha)*qp.z + rho_inv*y;


    if(qp.z.size() != tmp.size()){
        throw std::logic_error("Invalid vector size");
    }

    if(qp.l.size() != 0 && qp.u.size() != 0){

        if(qp.z.size() != qp.u.size()){
            throw std::logic_error("Invalid constraint vector size");
        }

        if(qp.z.size() != qp.l.size()){
            throw std::logic_error("Invalid constraint vector size");
        }

        // apply min/max constraints
        for(uint_t c=0; c<tmp.size(); ++c){
            qp.z[c] = std::min(std::max(tmp[c], qp.l[c]), qp.u[c]);
        }
    }
    else if(qp.u.size() != 0){

        if(qp.z.size() != qp.u.size()){
            throw std::logic_error("Invalid constraint vector size");
        }

        // apply max constraints
        for(uint_t c=0; c<tmp.size(); ++c){
            qp.z[c] = std::min(tmp[c], qp.u[c]);
        }
    }
    else if(qp.l.size() != 0){

        if(qp.z.size() != qp.l.size()){
            throw std::logic_error("Invalid constraint vector size");
        }

        // apply min constraints
        for(uint_t c=0; c<tmp.size(); ++c){
            qp.z[c] = std::max(tmp[c], qp.l[c]);
        }
    }
    else{
       qp.z = tmp;
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

    //std::cout<<y<<std::endl;

    DynVec<real_t> zold = qp.z;
    DynVec<real_t> rhs(qp.x.size() + qp.A.rows(), 0.0);

    // update the rhs
    update_direct_rhs_(rhs, y, qp);

    // update the direct matrix
    update_direct_matrix_(mat, qp);

    DynVec<real_t> z_tilda(qp.z.size(), 0.0);

    real_t rho_inv = 1.0/data_.rho;

    for(uint_t itr=0; itr<data_.max_n_iterations; ++itr){

        // solve linear system
        data_.solver.solve(mat, sol, rhs);

        // update z tilda vector
        for(uint_t c=0; c<qp.z.size(); ++c){
            z_tilda[c] = zold[c] + rho_inv*(sol[c + zold.size()] - y[c]);
        }

        // update solution
        for(uint_t c=0; c<qp.x.size(); ++c){
            qp.x[c] = data_.alpha*sol[c] +(1.0 - data_.alpha)*qp.x[c];
        }

        // constrain the vector
        project_vector_(qp, z_tilda, y);

        // update y vector
        for(uint_t c=0; c<y.size(); ++c){
            y[c] += data_.rho*(data_.alpha*z_tilda[c] + (1. - data_.alpha)*zold[c] - qp.z[c]);
        }

        // update rhs
        update_direct_rhs_(rhs, y, qp);

        // update zold vector
        zold = qp.z;
    }
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
   throw  std::logic_error("Not implemented");
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

template class ADMM<DynMat<real_t>, DynVec<real_t>>;

#ifdef USE_TRILINOS
template class ADMM<kernel::numerics::TrilinosEpetraMatrix,
                    kernel::maths::algebra::TrilinosEpetraMultiVector>;
#endif

}
}
}
