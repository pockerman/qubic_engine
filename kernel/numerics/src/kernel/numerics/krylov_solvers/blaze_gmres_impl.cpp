#include "kernel/numerics/krylov_solvers/blaze_gmres_impl.h"

#include <iostream> //std::cout below
namespace kernel{
namespace numerics{


BlazeGMRES::BlazeGMRES(const KrylovSolverData& data)
    :
      BlazeKrylovSolverImplBase(data)
{}

BlazeGMRES::output_t
BlazeGMRES::solve(const SparseMatrix<real_t>& A,
                  DynVec<real_t>& x, const DynVec<real_t>& b){

    /// the residual vector
    auto r0 = b-A*x;
    auto l2normr0 = blaze::l2Norm(r0);

    /// update the residual of the controller
    this->controller_.update_residual(l2normr0);

    /// matrix used for the orthogonalization process later
    DynMat<real_t> H(gmres_data_.n_tmp_vectors, gmres_data_.n_tmp_vectors - 1 );




    while(this->controller_.continue_iterations()){

        if(this->controller_.show_iterations()){
           std::cout<<this->controller_.get_state()<<std::endl;
        }

        auto v = r0/l2normr0;

        /// for the number of restarts
    }



}
}
}
