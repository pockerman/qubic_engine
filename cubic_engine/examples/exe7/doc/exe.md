# Example 7: Multithreaded Gradient Descent 

## Contents
* [Acknowledgements](#ackw)
* [Overview](#overview) 
* [Include files](#include_files)
* [Program structure](#prg_struct)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="ackw"></a>  Acknowledgements

## <a name="overview"></a> Overview

## <a name="include_files"></a> Include files

```
#include "kernel/base/config.h"

#ifdef USE_OPENMP
#include "kernel/parallel/threading/openmp_executor.h"
#endif

#include "kernel/base/kernel_consts.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/threaded_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"

#include <iostream>

```
## <a name="prg_struct"></a> Program structure

## <a name="m_func"></a> The main function

```
int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::ThreadedGd;
    using cengine::GDControl;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;
    using kernel::PartitionedType;

    const uint_t NUM_THREADS = 2;

    auto dataset = kernel::load_car_plant_dataset_with_partitions(2);

    GDControl control(10000, kernel::KernelConsts::tolerance(), GDControl::DEFAULT_LEARNING_RATE);
    control.show_iterations = true;

    ThreadedGd gd(control);

#ifdef USE_OPENMP
    {

        kernel::OMPExecutor executor(NUM_THREADS);

        //hypothesis of the form f = w_0 + w_1*x
        // where w_0 = w_1 = 0.0
        RealVectorPolynomialFunction hypothesis({0.0, 0.0});

        // the error functionto to use for measuring the error
        MSEFunction<RealVectorPolynomialFunction,
                PartitionedType<DynMat<real_t>>,
                PartitionedType<DynVec<real_t>>> mse(hypothesis);

        gd.solve(dataset.first, dataset.second, mse, hypothesis, executor, kernel::OMPOptions());
    }
#endif
    {
        kernel::ThreadPool executor(NUM_THREADS);

        //hypothesis of the form f = w_0 + w_1*x
        // where w_0 = w_1 = 0.0
        RealVectorPolynomialFunction hypothesis({0.0, 0.0});

        // the error functionto to use for measuring the error
        MSEFunction<RealVectorPolynomialFunction,
                PartitionedType<DynMat<real_t>>,
                PartitionedType<DynVec<real_t>>> mse(hypothesis);

        gd.solve(dataset.first, dataset.second, mse, hypothesis, executor, kernel::Null());

    }
        
    return 0;
}

```

## <a name="results"></a> Results

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exe.cpp</a>
