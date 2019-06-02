/**
  * Parallel Implementation of Jacobi Iteration
  *
  **/
#include "parframe/executors/thread_pool.h"
#include "parframe/models/reduce.h"
#include "parframe/models/reduction_operations.h"

#include <blaze/Math.h>

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>


namespace  {

    using parframe::uint_t;
    using parframe::real_t;
    using parframe::ThreadPool;
    using Vector = blaze::DynamicVector<real_t>;
    using Matrix = blaze::DynamicMatrix<real_t>;

}

int main(){

    using parframe::ThreadPool;
    Vector x(100, 1.0);

    //create a pool and start it with four threads
    ThreadPool pool(4);

    parframe::Sum<parframe::real_t> op;
    std::vector<parframe::range1d<Vector::Iterator>> partitions;
    parframe::reduce_array(partitions, op, pool);
    op.get();

    return 0;
}
