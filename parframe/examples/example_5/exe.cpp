/**
  * Parallel Implementation of Jacobi Iteration
  *
  **/
#include "parframe/base/types.h"
#include "parframe/executors/thread_pool.h"
#include "parframe/models/reduce.h"
#include "parframe/models/reduction_operations.h"
#include "parframe/partitioners/array_partitioner.h"

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>


namespace  {

    using kernel::uint_t;
    using kernel::real_t;
    using parframe::ThreadPool;
    using Vector = kernel::DynVec<real_t>;
}

int main(){

    try {

        using parframe::ThreadPool;
        Vector x(100, 1.0);

        //create a pool and start it with four threads
        ThreadPool pool(4);

        kernel::Sum<parframe::real_t> op;
        std::vector<parframe::range1d<Vector::Iterator>> partitions;

        //create the partitions
        partition_range(x.begin(), x.end(), partitions, pool.get_n_threads());

        parframe::reduce_array(partitions, op, pool);
        auto rslt = op.get();

        if( rslt.first != 100.0 && !rslt.second ){
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
