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

};

CGSolver::CGSolver(uint_t nitrs, real_t res)
    :
      n_itrs_(nitrs),
      res_(res)
{}

parframe::AlgInfo
CGSolver::solve(const Matrix& mat, const Vector& b, Vector& x, ThreadPool& executor ){

    // do basic checks

    // allocate helper vectors
    Vector g;
    Vector d;

    for(uint itr = 0; itr < n_itrs_; ++itr){



    }
}


}

int main(){

    try {

        using parframe::ThreadPool;
        Vector x(100, 1.0);

        //create a pool and start it with four threads
        ThreadPool pool(4);

        parframe::Sum<parframe::real_t> op;
        std::vector<parframe::range1d<Vector::Iterator>> partitions;

        //create the partitions
        partition_range(x.begin(), x.end(), partitions, pool.get_n_threads());

        parframe::reduce_array(partitions, op, pool);
        auto rslt = op.get();

        if( rslt.first != 100.0 && rslt.second == false){
            std::cout<<"Result: "<<rslt.first<<" should be 100"<<std::endl;
            throw std::logic_error("Incorrect result");
        }
        else{
            std::cout<<"Result: "<<rslt.first<<" is correct and valid "<<rslt.second<<std::endl;
        }
    }
    catch (std::logic_error& e) {
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}
