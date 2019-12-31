# Example: Calculating the Sum Squared Error

## Contents
* [Acknowledgements](#ackw)
* [Overview](#overview) 
	* [Sum Squared Error](#sum_squared_error)
* [Include files](#include_files)
* [Program structure](#prg_struct)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="ackw"></a>  Acknowledgements

## <a name="overview"></a> Overview

### <a name="sum_squared_error"></a> Sum Squared Error

## <a name="include_files"></a> Include files

```
#include "kernel/base/config.h"

#include "kernel/base/types.h"
#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include "kernel/parallel/utilities/reduction_operations.h"

#ifdef USE_OPENMP
#include "kernel/parallel/threading/openmp_executor.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#endif

#include <iostream>
```

## <a name="prg_struct"></a> Program structure

## <a name="m_func"></a> The main function

```
innt main(){

    using kernel::real_t;
    using kernel::uint_t;
    using kernel::RealVectorPolynomialFunction;
    using kernel::SSEFunction;
    using DynMat = kernel::DynMat<real_t>;
    using DynVec = kernel::DynVec<real_t>;

    {
        // the hypothesis function y = w_0 + w_1*x
        RealVectorPolynomialFunction function({0.409, 0.499});

        // the Mean Squared Error calculation
        SSEFunction<RealVectorPolynomialFunction, DynMat, DynVec> sse(function);

        // load the data set
        std::pair<DynMat, DynVec> data_set = kernel::load_car_plant_dataset();

        auto value = sse.value(data_set.first, data_set.second);
        std::cout<<"SSE error is: "<<value.get_resource()<<std::endl;
    }

#ifdef USE_OPENMP
    {

        // use two threads
        kernel::OMPExecutor executor(2);

        // the hypothesis function y = w_0 + w_1*x
        RealVectorPolynomialFunction function({0.409, 0.499});

        // the Mean Squared Error calculation
        SSEFunction<RealVectorPolynomialFunction,
                    kernel::PartitionedType<DynMat>,
                    kernel::PartitionedType<DynVec> > sse(function);

        // load the data set
        auto data_set = kernel::load_car_plant_dataset_with_partitions(executor.get_n_threads());

        auto value = sse.value(data_set.first, data_set.second, executor, kernel::OMPOptions());
        std::cout<<"SSE error is: "<<value.get_resource()<<std::endl;
    }

#endif
    return 0;

}
```

## <a name="results"></a> Results

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




