#include "kernel/base/config.h"

#ifdef USE_FEM

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/numerics/optimization/serial_gradient_descent.h"
#include "kernel/maths/functions/function_base.h"

#include <iostream>

namespace example {

using kernel::real_t;
using kernel::uint_t;
using kernel::DynMat;
using kernel::DynVec;
using kernel::maths::opt::Gd;
using kernel::maths::opt::GDConfig;

class Function: public kernel::FunctionBase<real_t, DynVec<real_t>>
{
public:

    typedef kernel::FunctionBase<real_t, DynVec<real_t>>::output_t output_t;

    // constructor
    Function(const DynVec<real_t>& coeffs);

    // compute the value of the function
    virtual output_t value(const DynVec<real_t>&  input)const override final;

    // compute the gradients of the function
    virtual DynVec<real_t> gradients(const DynVec<real_t>&  input)const override final;

    // the number of coefficients
    virtual uint_t n_coeffs()const override final{return 2;}

    // reset the coefficients
    void set_coeffs(const DynVec<real_t>&  coeffs){coeffs_ = coeffs;}

    // get a copy of the coefficients
    DynVec<real_t> coeffs()const{return coeffs_;}

private:

    // coefficients vector
    DynVec<real_t> coeffs_;

};

Function::Function(const DynVec<real_t>& coeffs)
    :
      coeffs_(coeffs)
{}

Function::output_t
Function::value(const DynVec<real_t>&  input)const{
    return 0.5*(kernel::utils::sqr(kernel::utils::sqr(input[0]) - input[1])) +
           0.5*(kernel::utils::sqr(input[0] - 1.0));
}

DynVec<real_t>
Function::gradients(const DynVec<real_t>&  input)const{

    auto grad1= 2.0*input[0]*(kernel::utils::sqr(input[0]) - input[1]) + (input[0] - 1.0);
    auto grad2 = -(kernel::utils::sqr(input[0]) - input[1]);
    DynVec<real_t> rslt(2, 0.0);
    rslt[0] = grad1;
    rslt[1] = grad2;
    return rslt;
}

}

int main(){

    using namespace example;
    try{

        GDConfig config(20, kernel::KernelConsts::tolerance(), 0.1);
        config.set_show_iterations_flag(true);
        Gd gd(config);

        DynVec<real_t> coeffs(2, 0.0);

        Function f(coeffs);

        auto info = gd.solve(f);
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
#else
#include <iostream>

int main(){

    std::cout<<"The example requires FEM support. Configure kernel_numerics with FEM support"<<std::endl;
}
#endif





