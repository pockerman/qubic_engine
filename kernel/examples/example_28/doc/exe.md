# Example 26: Newton's Method

## Contents

* [Overview](#overview) 
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="overview"></a> Overview

## <a name="include_files"></a> Include files

```
#include "kernel/base/types.h"
#include "kernel/maths/solvers/newton_solver.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/functions/numeric_scalar_function.h"

#include <cmath>
#include <iostream>
```

## <a name="m_func"></a> The main function

```
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
```

## <a name="results"></a> Results

```
Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 1.79769e+308
Iterations..: 1

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 0.200285
Iterations..: 2

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 0.0523745
Iterations..: 3

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 0.0144196
Iterations..: 4

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 0.00402365
Iterations..: 5

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 0.00112692
Iterations..: 6

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 0.000315948
Iterations..: 7

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 8.86057e-05
Iterations..: 8

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 2.4851e-05
Iterations..: 9

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 6.97004e-06
Iterations..: 10

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 1.95492e-06
Iterations..: 11

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 5.48309e-07
Iterations..: 12

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 1.53787e-07
Iterations..: 13

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 4.31337e-08
Iterations..: 14

Iteration state: Converged...: false
Tolerance...: 1e-08
Residual....: 1.2098e-08
Iterations..: 15

Converged...: true
Tolerance...: 1e-08
Residual....: 3.39318e-09
Iterations..: 16

( 0.360422 )
```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




