# Example 5: Simulate Standard Error For The Mean 

## Contents
* [Overview](#overview) 
		* [Standard Error For The Mean](#standard_error_for_mean)
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)


### <a name="standard_error_for_mean"></a> Standard Error For The Mean

## <a name="include_files"></a> Include files
```
#include "kernel/base/types.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/parallel_algos/parallel_reduce.h"
#include "kernel/parallel/utilities/reduction_operations.h"
#include "kernel/parallel/utilities/array_partitioner.h"

#include <thread>
#include <vector>
#include <iostream>
#include <stdexcept>
```

## <a name="m_func"></a> The main function
```
namespace  {

    using kernel::uint_t;
    using kernel::real_t;
    using kernel::ThreadPool;
    using Vector = kernel::DynVec<real_t>;
}

int main(){

    try {

        using kernel::ThreadPool;
        Vector x(100, 1.0);

        //create a pool and start it with four threads
        ThreadPool pool(4);

        kernel::Sum<kernel::real_t> op;
        std::vector<kernel::range1d<Vector::Iterator>> partitions;

        //create the partitions
        partition_range(x.begin(), x.end(), partitions, pool.get_n_threads());

        kernel::reduce_array(partitions, op, pool);
        auto rslt = op.get();

        if( *rslt.first != 100.0 && !rslt.second ){
            std::cout<<"Result: "<<*rslt.first<<" should be 100"<<std::endl;
            throw std::logic_error("Incorrect result");
        }
        else{
            std::cout<<"Result: "<<*rslt.first<<" is correct and valid "<<rslt.second<<std::endl;
        }
    }
    catch (std::logic_error& e) {
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}
```

## <a name="results"></a> Results

Upon executing the driver program an output similar to the output
below should be produced.

```
Standard error for mean is: 0.323156, N/sqrt(N): 0.316228
```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exe.cpp</a>




