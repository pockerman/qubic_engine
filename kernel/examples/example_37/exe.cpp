#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/maths/optimization/stochastic_gradient_descent.h"
#include "kernel/maths/errorfunctions/sse_function.h"
#include "kernel/maths/functions/real_vector_polynomial.h"

#include <cmath>
#include <iostream>
#include <string>
#include <utility>

namespace example {

using kernel::real_t;
using kernel::uint_t;
using kernel::DynMat;
using kernel::DynVec;
using kernel::RealVectorPolynomialFunction;
using kernel::SSEFunction;
using kernel::maths::opt::SGD;
using kernel::maths::opt::GDConfig;

std::pair<DynMat<real_t>, DynVec<real_t>> create_data(){

    DynMat<real_t> mat(10, 2);
    DynVec<real_t> vec(10);
    return {mat, vec};
}


}

int main(){

    using namespace example;
    try{

        typedef  SSEFunction<RealVectorPolynomialFunction, DynMat<real_t>, DynVec<real_t>> error_t;
        auto data = create_data();
        GDConfig config(5, kernel::KernelConsts::tolerance(), 0.1);
        SGD sgd(config);

        std::vector<real_t> coeffs(2, 0.0);

        // the model to use
        RealVectorPolynomialFunction model(coeffs);

        // the error function
        error_t err_func(model);

        sgd.solve(data.first, data.second, err_func, model);

    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





