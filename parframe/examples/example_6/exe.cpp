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
    kernel::DotProduct<Vector, real_t> rr_dotproduct(r_rsult, r_rsult);

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

    std::cout<<"r_0: "<<std::endl;
    std::cout<<r_rsult.get_resource()<<std::endl;

    std::cout<<"g: "<<std::endl;
    std::cout<<g_rsult.get_resource()<<std::endl;

    for(uint itr = 0; itr < n_itrs_; ++itr){

        info.niterations = itr +1 ;

        // w = compute A*g
        A_times_g.reexecute(executor);
        A_times_g.get_or_wait_copy(w_rsult);

        //std::cout<<"A*g result: "<<std::endl;
        //std::cout<<w_rsult.get_resource()<<std::endl;

        gw_dotproduct.reexecute(executor);
        auto& gw_dot = gw_dotproduct.get_or_wait();

        //std::cout<<"g*w dot product: "<<std::endl;
        //std::cout<<*gw_dot.get().first<<std::endl;

        rr_dotproduct.reexecute(executor);
        auto& result_2 = rr_dotproduct.get_or_wait();

        // hold a copy of the just computed gTg product
        old_r_dot_product = *(result_2.get().first);

        //std::cout<<" r*r dot product: "<<std::endl;
        //std::cout<<old_r_dot_product<<std::endl;

        // compute alpha
        alpha = old_r_dot_product / *(gw_dot.get().first);

        //std::cout<<"Alpha computed: "<<std::endl;
        //std::cout<<alpha<<std::endl;

        // update solution x
        std::cout<<"Update x solution"<<std::endl;
        update_x.reexecute(executor);

        std::cout<<"Updated solution: "<<std::endl;
        std::cout<<x_rsult.get_resource()<<std::endl;

        // update residual r
        update_r.reexecute(executor);
        std::cout<<"Residual computed: "<<std::endl;
        std::cout<<r_rsult.get_resource()<<std::endl;

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

        using parframe::ThreadPool;
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
        ThreadPool pool(1);

        //create the partitions
        std::vector<parframe::range1d<uint_t>> partitions;
        uint_t start =0;
        uint_t end = A.rows();
        parframe::partition_range(start, end, partitions, pool.get_n_threads());
        A.set_partitions(partitions);
        x.set_partitions(partitions);
        b.set_partitions(partitions);

        CGSolver solver(1, kernel::kernel_consts::tolerance());
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
