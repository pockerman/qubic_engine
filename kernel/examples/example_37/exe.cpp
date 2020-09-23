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
    mat(0,0)=2.7810836;
    mat(0,1)=2.550537003;

    mat(1,0)=1.465489372;
    mat(1,1)=2.362125076;

    mat(2,0)=3.396561688;
    mat(2,1)=4.400293529;

    mat(3,0)=1.38807019;
    mat(3,1)=1.850220317;

    mat(4,0)=3.06407232;
    mat(4,1)=3.005305973;

    mat(5,0)=7.627531214;
    mat(5,1)=2.759262235;

    mat(6,0)=5.332441248;
    mat(6,1)=2.088626775;

    mat(7,0)=6.922596716;
    mat(7,1)=1.77106367;

    mat(8,0)=8.675418651;
    mat(8,1)=-0.242068655;

    mat(9,0)=7.673756466;
    mat(9,1)=3.508563011;

    DynVec<real_t> vec(10, 0.0);
    for(uint_t idx=5; idx<vec.size(); ++idx){
       vec[idx] = 1.0;
    }

    return {mat, vec};
}


}

int main(){

    using namespace example;
    try{

        typedef  SSEFunction<RealVectorPolynomialFunction, DynMat<real_t>, DynVec<real_t>> error_t;
        auto data = create_data();
        GDConfig config(10, kernel::KernelConsts::tolerance(), 0.01);
        config.set_show_iterations_flag(true);
        SGD sgd(config);

        std::vector<int> order_coeffs(2, 0);
        order_coeffs[1] = 1;

        DynVec<real_t> coeffs(2, 0.0);

        // the model to use
        RealVectorPolynomialFunction model(coeffs, order_coeffs);

        // the error function
        error_t err_func(model);

        auto info = sgd.solve(data.first, data.second, err_func, model);
        std::cout<<info<<std::endl;

    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





