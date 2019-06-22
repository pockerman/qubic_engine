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

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>


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
    parframe::AlgInfo solve(const Matrix& mat, const Vector& b, Vector& x, ThreadPool& executor );


private:

    uint_t n_itrs_;
    real_t res_;

    struct MatVecProduct;

    std::vector<std::unique_ptr<MatVecProduct>> mat_vec_tasks_;

    // structure responsible for computing the
    // matrix-vector product
    struct MatVecProduct: public parframe::SimpleTaskBase
    {

        MatVecProduct(uint_t id, const Matrix& mat, const Vector& x, Vector& rslt)
            :
        parframe::SimpleTaskBase(id),
        mat_ptr(&mat),
        x_ptr(&x),
        rslt_ptr(&rslt)
        {}

        const Matrix* mat_ptr;
        const Vector* x_ptr;
        Vector* rslt_ptr;

        // execute the matrix-vector product
        virtual void run()override final;
    };

};


void
CGSolver::MatVecProduct::run(){

    // get the rows partiton indeces corresponding to this task
    const parframe::range1d<uint_t> parts = mat_ptr->get_partition(this->get_id());

    auto begin = parts.begin();
    auto end   = parts.end();

    for(uint_t r  = begin; r < end; ++r){
        auto& rslt = rslt_ptr->operator[](r);

        for(Matrix::ConstIterator it = mat_ptr->begin(r); it != mat_ptr->end(r); ++it )  {
           rslt += *it*x_ptr->operator[](r);
        }
    }
}

CGSolver::CGSolver(uint_t nitrs, real_t res)
    :
      n_itrs_(nitrs),
      res_(res)
{}

parframe::AlgInfo
CGSolver::solve(const Matrix& mat, const Vector& b, Vector& x, ThreadPool& executor ){

    // do basic checks

    parframe::AlgInfo info;
    info.nthreads = executor.get_n_threads();

    // allocate helper vectors
    Vector g(x.size());
    Vector d(x.size());
    Vector w(x.size());

    mat_vec_tasks_.reserve(executor.get_n_threads());

    // create the tasks
    for(uint_t t=0; t<executor.get_n_threads(); ++t){
        mat_vec_tasks_.push_back(std::make_unique<MatVecProduct>(t, mat, x, w));
    }

    for(uint itr = 0; itr < n_itrs_; ++itr){


        info.niterations = itr;

    }

    return info;
}

}

int main(){

    try {

        using parframe::ThreadPool;
        Vector x(100, 0.0);
        Vector b(100, 0);
        Matrix A(100, 100);

        //create a pool and start it with four threads
        ThreadPool pool(4);

        parframe::Sum<parframe::real_t> op;
        std::vector<parframe::range1d<uint_t>> partitions;

        //create the partitions
        uint_t start =0;
        uint_t end = A.rows();
        parframe::partition_range(start, end, partitions, pool.get_n_threads());

        CGSolver solver(100, parframe::kernel_consts::tolerance());
        solver.solve(A, x, b, pool);
        //parframe::reduce_array(partitions, op, pool);

    }
    catch (std::logic_error& e) {
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}
