#include "kernel/base/config.h"

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/parallel/threading/task_base.h"
#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include "kernel/parallel/utilities/reduction_operations.h"

#ifdef USE_OPENMP
#include "kernel/parallel/threading/openmp_executor.h"
#endif

#include <random>
#include <iostream>

namespace exe
{
using kernel::real_t;
using kernel::uint_t;
using kernel::RealVectorPolynomialFunction;
using kernel::MSEFunction;
using DynMat = kernel::DynMat<real_t>;
using DynVec = kernel::DynVec<real_t>;

}

int main(){

    using namespace exe;

    // the hypothesis function y = w_0 + w_1*x
    RealVectorPolynomialFunction function({0.409, 0.499});

    // the Mean Squared Error calculation
    MSEFunction<RealVectorPolynomialFunction, DynMat, DynVec> mse(function);

    // load the data set
    std::pair<DynMat, DynVec> data_set = kernel::load_car_plant_dataset();

    auto value = mse.value(data_set.first, data_set.second);
    std::cout<<"MSE error is: "<<value<<std::endl;

    return 0;
}




