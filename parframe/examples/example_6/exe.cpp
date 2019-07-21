/**
  * Parallel Implementation of Jacobi Iteration
  *
  **/
#include "parframe/base/types.h"
#include "parframe/executors/thread_pool.h"
#include "parframe/models/reduce.h"
#include "parframe/models/reduction_operations.h"

#include "parframe/partitioners/array_partitioner.h"
#include "parframe/base/algorithm_info.h"
#include "parframe/data_structs/partitioned_object.h"
#include "parframe/executors/simple_task.h"
#include "parframe/models/vector_updater.h"
#include "parframe/models/scaled_ops.h"
#include "parframe/models/linear_algebra/dot_product.h"
#include "parframe/models/linear_algebra/matrix_vector_product.h"

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>


namespace  {

    using kernel::uint_t;
    using kernel::real_t;
    using parframe::ThreadPool;
    using ThreadPool = parframe::ThreadPool;
    using Matrix = parframe::PartitionedType<kernel::DynMat<real_t>>;
    using Vector = parframe::PartitionedType<kernel::DynVec<real_t>>;

class CGSolver
{

public:

    // Constructor
    CGSolver(uint_t nitrs, real_t res);

    // solve the Ax=b. It assumes that the matrix and Vector
    // types already have their partitions established
    kernel::AlgInfo solve(const Matrix& mat, const Vector& b, Vector& x, ThreadPool& executor );

private:

    uint_t n_itrs_;
    real_t res_;
};



CGSolver::CGSolver(uint_t nitrs, real_t res)
    :
      n_itrs_(nitrs),
      res_(res)
{}

kernel::AlgInfo
CGSolver::solve(const Matrix& mat, const Vector& b, Vector& x, ThreadPool& executor ){

    // do basic checks
    kernel::AlgInfo info;
    info.nthreads = executor.get_n_threads();

    // allocate helper vectors
    Vector g(x.size());
    Vector d(x.size());
    Vector w(x.size());

    real_t alpha = 0.0;
    real_t beta = 0.0;

    // object to perform the dot product
    kernel::MatVecProduct<Matrix, Vector> A_times_d(mat, d);
    kernel::MatVecProduct<Matrix, Vector> A_times_b(mat, b);
    kernel::DotProduct<Vector, real_t> gg_dotproduct(g,g);
    kernel::DotProduct<Vector, real_t> dw_dotproduct(d, w);

    kernel::ResultHolder<Vector> xrsult(std::move(x));
    kernel::ResultHolder<Vector> grsult(std::move(g));
    kernel::ResultHolder<Vector> drsult(std::move(d));

    kernel::VectorUpdater<Vector, kernel::ScaledSum<real_t>, real_t> update_x(xrsult, x, d, 1.0, alpha);
    kernel::VectorUpdater<Vector, kernel::ScaledSum<real_t>, real_t> update_g(grsult, g, w, 1.0, alpha);
    kernel::VectorUpdater<Vector, kernel::ScaledSum<real_t>, real_t> update_d(drsult, d, w, -1.0, beta);

    A_times_b.execute(executor);
    auto& r = A_times_b.get_or_wait();

    for(uint itr = 0; itr < n_itrs_; ++itr){

        // perform the matrix-vector product
        A_times_d.reexecute(executor);

        dw_dotproduct.reexecute(executor);
        auto& result_1 = dw_dotproduct.get_or_wait();

        gg_dotproduct.reexecute(executor);
        auto& result_2 = gg_dotproduct.get_or_wait();

        // compute alpha
        alpha = *(result_2.get().first) / *(result_1.get().first);

        // update g and x
        update_x.reexecute(executor);
        update_g.reexecute(executor);

        gg_dotproduct.reexecute(executor);
        auto& result_3 = gg_dotproduct.get_or_wait();
        beta = *(result_3.get().first) / *(result_2.get().first);

        // update d vector
        update_d.reexecute(executor);

        info.niterations = itr;
        auto res = std::sqrt( *(result_3.get().first) );

        if( res - res_ < 0.){
            info.converged = true;
            info.residual = res;
        }
    }

    return info;
}

}

int main(){

    try {

        using parframe::ThreadPool;
        Vector x(100, 0.0);
        Vector b(100, 2.0);
        Matrix A(100, 100);

        // diagonilize A
        for(uint_t r=0; r < A.rows(); ++r){
            for(uint_t c=0; c < A.columns(); ++c){

                if(r == c){
                    A(r,c) = 1.0;
                }
            }
        }

        //create a pool and start it with four threads
        ThreadPool pool(4);

        kernel::Sum<parframe::real_t> op;
        std::vector<parframe::range1d<uint_t>> partitions;

        //create the partitions
        uint_t start =0;
        uint_t end = A.rows();
        parframe::partition_range(start, end, partitions, pool.get_n_threads());

        CGSolver solver(100, kernel::kernel_consts::tolerance());
        auto info = solver.solve(A, x, b, pool);

        // print useful information
        std::cout<<info<<std::endl;

        // uncomment this to view the solution
        //std::cout<<x<<std::endl;
    }
    catch (std::logic_error& e) {
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}
