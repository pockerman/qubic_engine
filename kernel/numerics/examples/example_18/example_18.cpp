#include "kernel/base/types.h"
#include "kernel/numerics/solvers/newton_solver.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/functions/numeric_scalar_function.h"

#include <cmath>
#include <iostream>

namespace example
{
using kernel::real_t;
using kernel::uint_t;
using kernel::GeomPoint;
using kernel::DynVec;

class Function: public kernel::numerics::NumericScalarFunction<1>
{
public:

    Function();

    /// Returns the value of the function
    virtual output_t value(const GeomPoint<1>&  x)const override{
       return 3.0*x[0] + std::sin(x[0]) - std::exp(x[0]);
    }

    /// Returns the gradients of the function
    virtual DynVec<real_t> gradients(const GeomPoint<1>&  x)const override{
        return DynVec<real_t>(1, 3.0 + std::cos(x[0] - std::exp(x[0])));
     }

};

Function::Function()
    :
   kernel::numerics::NumericScalarFunction<1>()
{}

}

int main(){

    using namespace example;

    try{

        const uint_t N_ITRS = 1000;
        const real_t TOL = kernel::KernelConsts::tolerance();

        Function f;

        kernel::maths::solvers::NewtonSolver<GeomPoint<1>, Function> newton(N_ITRS, TOL);
        newton.set_show_iterations_flag(true);
        auto [output, solution] = newton.solve(kernel::GeomPoint<1>({0.0}),f);

        std::cout<<output<<std::endl;
        std::cout<<solution<<std::endl;

    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





