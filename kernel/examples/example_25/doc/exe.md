# Example 25: Bisection Method

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
#include "kernel/maths/solvers/bisection_solver.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/base/kernel_consts.h"

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

}

int main(){

    using namespace example;

    try{

        const uint_t N_ITRS = 1000;
        const real_t TOL = kernel::KernelConsts::tolerance();

        auto f = [&](const kernel::GeomPoint<1> x){
            return 3.0*x[0] + std::sin(x[0]) - std::exp(x[0]);
        };

        kernel::maths::solvers::BisectionSolver<GeomPoint<1>, decltype (f)> bs(N_ITRS, TOL);
        auto [output, solution] = bs.solve(kernel::GeomPoint<1>({0.0}),
                                           kernel::GeomPoint<1>({1.0}), f);

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
Converged...: true
Tolerance...: 1e-08
Residual....: 7.45058e-09
Iterations..: 28

( 0.360422 )
```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




