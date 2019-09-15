/**
  * Parallel Implementation of Jacobi Iteration
  *
  **/
#include "kernel/base/types.h"
#include "kernel/base/algorithm_info.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/parallel_algos/parallel_reduce.h"
#include "kernel/parallel/parallel_algos/reduction_operations.h"
#include "kernel/parallel/utilities/array_partitioner.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/parallel/parallel_algos/linear_algebra/dot_product.h"
#include "kernel/parallel/parallel_algos/linear_algebra/matrix_vector_product.h"
#include "kernel/parallel/parallel_algos/vector_updater.h"
#include "kernel/utilities/scaled_ops.h"


#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cmath>


namespace  {

    using kernel::uint_t;
    using kernel::real_t;
    using kernel::ThreadPool;
    using ThreadPool = kernel::ThreadPool;
    using Matrix = kernel::PartitionedType<kernel::DynMat<real_t>>;
    using Vector = kernel::PartitionedType<kernel::DynVec<real_t>>;

class CGSolver
{

public:

    // Constructor
    CGSolver(uint_t nitrs, real_t res);

    // solve the Ax=b. It assumes that the matrix and Vector
    // types already have their partitions established
    kernel::AlgInfo solve(const Matrix& mat, Vector& x, const Vector& b, ThreadPool& executor );

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
CGSolver::solve(const Matrix& mat, Vector& x, const Vector& b, ThreadPool& executor ){

    // do basic checks
    kernel::AlgInfo info;
    info.nthreads = executor.get_n_threads();

    // allocate helper vectors
    Vector g(x.size());
    g.set_partitions(b.get_partitions());
    Vector r(x.size());
    r.set_partitions(b.get_partitions());

    const auto one = 1.0;
    auto alpha = 0.0;
    auto beta = 0.0;
    auto old_r_dot_product = 0.0;

    kernel::ResultHolder<Vector> x_rsult(std::move(x));
    kernel::ResultHolder<Vector> g_rsult(std::move(g));
    kernel::ResultHolder<Vector> r_rsult(std::move(r));
    kernel::ResultHolder<Vector> w_rsult;

    // object to perform the dot product
    kernel::MatVecProduct<Matrix, Vector> A_times_g(mat, g_rsult.get_resource());
    A_times_g.get_copy(w_rsult);

    kernel::DotProduct<Vector, real_t> gg_dotproduct(g_rsult, g_rsult);
    kernel::DotProduct<Vector, real_t> gw_dotproduct(g_rsult, w_rsult);
    kernel::DotProduct<Vector, real_t> rr_dotproduct(r_rsult);

    // update the solution vector
    kernel::VectorUpdater<Vector, kernel::ScaledSum<real_t>, real_t> update_x(x_rsult, x_rsult.get_resource(), g_rsult.get_resource(), one, alpha);
    kernel::VectorUpdater<Vector, kernel::ScaledSum<real_t>, real_t> update_g(g_rsult, r_rsult.get_resource(), g_rsult.get_resource(), one, beta);

    // update the residual vector
    kernel::VectorUpdater<Vector, kernel::ScaledDif<real_t>, real_t> update_r(r_rsult, r_rsult.get_resource(), g_rsult.get_resource(), one, alpha);

    {
        // compute A*x_0
        kernel::MatVecProduct<Matrix, Vector> A_times_x(mat, x_rsult.get_resource());

        // compute A*x_0
        A_times_x.execute(executor);
        auto& tmp = A_times_x.get_or_wait();

        // compute r_0 = b - A*x_0
        kernel::VectorUpdater<Vector, kernel::ScaledDif<real_t>, real_t> update_r0(r_rsult, b, tmp.get_resource(), 1.0, 1.0);
        update_r0.execute(executor);
    }

    // initialize g vector with r vector
    g_rsult.get_resource() = r_rsult.get_resource();

    for(uint itr = 0; itr < n_itrs_; ++itr){

        info.niterations = itr +1 ;

        // w = compute A*g
        A_times_g.reexecute(executor);
        A_times_g.get_or_wait_copy(w_rsult);

        gw_dotproduct.reexecute(executor);
        auto& gw_dot = gw_dotproduct.get_or_wait();

        rr_dotproduct.reexecute(executor);
        auto& result_2 = rr_dotproduct.get_or_wait();

        // hold a copy of the just computed gTg product
        old_r_dot_product = *(result_2.get().first);

        // compute alpha
        alpha = old_r_dot_product / *(gw_dot.get().first);

        // update solution x
        update_x.reexecute(executor);

        // update residual r
        update_r.reexecute(executor);

        // check whether we converged... compute L2 norm of residual
        rr_dotproduct.reexecute(executor);
        auto& result_rr_dot_product = rr_dotproduct.get_or_wait();
        auto res = std::sqrt( *result_rr_dot_product.get().first );
        std::cout<<"Residual computed: "<<res<<" needed for convergence: "<<res_<<std::endl;

        if( res < res_ ){
            info.converged = true;
            info.residual = res;
            break; // iterations converged so break
        }

        // compute beta
        beta = *(result_rr_dot_product.get().first) / old_r_dot_product;

        // update g
        update_g.reexecute(executor);
    }

    x = std::move(x_rsult.get_resource());
    return info;
}

}

int main(){

    try {

        using kernel::ThreadPool;
        Vector x(5, 0.0);
        Vector b(5, 2.0);
        Matrix A(5, 5);

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

        //create the partitions
        std::vector<kernel::range1d<uint_t>> partitions;
        uint_t start =0;
        uint_t end = A.rows();
        kernel::partition_range(start, end, partitions, pool.get_n_threads());
        A.set_partitions(partitions);
        x.set_partitions(partitions);
        b.set_partitions(partitions);

        CGSolver solver(1, kernel::kernel_consts::tolerance());
        auto info = solver.solve(A, x, b, pool);

        // print useful information
        std::cout<<info<<std::endl;

        // uncomment this to view the solution
        std::cout<<x<<std::endl;
    }
    catch (std::logic_error& e) {
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}
