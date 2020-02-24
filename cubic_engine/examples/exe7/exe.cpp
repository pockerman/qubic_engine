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

    auto dataset = kernel::load_car_plant_dataset_with_partitions(NUM_THREADS);
    typedef RealVectorPolynomialFunction hypothesis_t;
    typedef MSEFunction<hypothesis_t, PartitionedType<DynMat<real_t>>, PartitionedType<DynVec<real_t>>> error_t;


#ifdef USE_OPENMP
    {

        kernel::OMPExecutor executor(NUM_THREADS);

        //hypothesis of the form f = w_0 + w_1*x
        // where w_0 = w_1 = 0.0
        hypothesis_t hypothesis({0.0, 0.0});

        GDControl control(10000, kernel::KernelConsts::tolerance(),
                                   GDControl::DEFAULT_LEARNING_RATE);

        ThreadedGd<error_t> gd(control);

        auto result = gd.solve(dataset.first, dataset.second, hypothesis, executor, kernel::OMPOptions());
        std::cout<<result<<std::endl;
        std::cout<<"Intercept: "<<hypothesis.coeff(0)<<", slope: "<<hypothesis.coeff(1)<<std::endl;
        std::cout<<std::endl;
    }
#endif
    {
        kernel::ThreadPool executor(NUM_THREADS);

        //hypothesis of the form f = w_0 + w_1*x
        // where w_0 = w_1 = 0.0
        hypothesis_t hypothesis({0.0, 0.0});

        GDControl control(10000, kernel::KernelConsts::tolerance(),
                                   GDControl::DEFAULT_LEARNING_RATE);

        ThreadedGd<error_t> gd(control);

        auto result = gd.solve(dataset.first, dataset.second, hypothesis, executor, kernel::Null());
        std::cout<<result<<std::endl;
        std::cout<<"Intercept: "<<hypothesis.coeff(0)<<", slope: "<<hypothesis.coeff(1)<<std::endl;
    }
        
    return 0;
}

