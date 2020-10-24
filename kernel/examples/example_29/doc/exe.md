# Example 26: Markov Chain Sampling

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
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"

#include <cmath>
#include <iostream>
```

## <a name="m_func"></a> The main function

```
int main(){

    try{

            using kernel::real_t;
            using kernel::uint_t;

            const uint_t N_ITERATIONS = 10000;
            const real_t delta = 0.1;
            real_t x = 1.0;
            real_t y = 1.0;

            std::random_device rd;
            std::mt19937 gen(rd());

            /// distribution of the x-coordinate
            std::uniform_real_distribution<real_t> x_distribution(-delta, delta);
            std::uniform_real_distribution<real_t> y_distribution(-delta, delta);

            real_t area_under_curve = 0.0;

            for(uint_t itr=0; itr < N_ITERATIONS; ++itr){

               real_t del_x = x_distribution(gen);
               real_t del_y = y_distribution(gen);

               if(std::fabs(x+ del_x) < 1.0 && std::fabs(y + del_y) < 1.0){
                   x += del_x;
                   y += del_y;
               }

               if(kernel::utils::sqr(x) + kernel::utils::sqr(y) < 1.0){
                  area_under_curve += 1;
               }
             }

             std::cout<<"Pi is: "<<4.0* area_under_curve/static_cast<real_t>(N_ITERATIONS);

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
Pi is: 3.136
```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




