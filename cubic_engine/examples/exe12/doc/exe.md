# Example 12: Linear Regression

## Contents
* [Acknowledgements](#ackw)
* [Overview](#overview) 
                * [Linear Regression](#linear_regression)
                * [How Good Is The Fit?](#how_good_is_the_fit)
                        *[```R^2``` Coefficient](#r2_coefficient)
* [Include files](#include_files)
* [Program structure](#prg_struct)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="overview"></a> Overview

This example discusses the linear regression model. In Statistics, linear regression is a mathematical approach to modeling
the relationship between a scalar response (or dependent variable) and one or more explanatory variables (or independent variables).
The case of one explanatory variable is called simple linear regression.  For more than one explanatory variable, the process is called multiple linear regression
(checkout the wikipedia article <a href="https://en.wikipedia.org/wiki/Linear_regression">Linear regression</a>).

Contrary to classification that is concerned with
class indexes, the outcome of a linear regression model or more general of a regression model is a real number.

### <a name="linear_regression"></a> Linear Regression

![LinearRegression](linear_regression.png)

### <a name="how_good_is_the_fit"></a> How Good Is The Fit?

So we established the linear regression  model but how can we measure how good it is?
One metric to do so is the so-called ```R^2``` Coefficient also called the Coefficient of determination

#### <a name="r2_coefficient"></a> ```R^2``` Coefficient

The coefficient is defined as

![R2](r2.gif)

where  SSR and SST are defined respectively as


![SSR](ssr.gif)

![SST](sst.gif)

## <a name="include_files"></a> Include files

```
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/regressor.h"
#include "cubic_engine/optimization/serial_batch_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"

#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include <iostream>
```

## <a name="m_func"></a> The main function

```
int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::GDControl;
    using cengine::Gd;
    using cengine::LinearRegression;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;

    try{

        auto dataset = kernel::load_car_plant_dataset();

        // The regressor to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1
        // set initial weights to 0
        LinearRegression regressor({0.0, 0.0});

        // the error function to to use for measuring the error
        MSEFunction<RealVectorPolynomialFunction,
                    DynMat<real_t>,
                    DynVec<uint_t>> mse(regressor.get_model());

        GDControl control(10000, kernel::KernelConsts::tolerance(), GDControl::DEFAULT_LEARNING_RATE);
        control.show_iterations = false;
        Gd gd(control);

        auto result = regressor.train(dataset.first, dataset.second, gd, mse);
        std::cout<<result<<std::endl;

        std::cout<<"Intercept: "<<regressor.coeff(0)<<" slope: "<<regressor.coeff(1)<<std::endl;

        DynVec<real_t> point{1.0, 5.7};
        auto value = regressor.predict(point);

        std::cout<<"Value: "<<value<<std::endl;
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

```

## <a name="results"></a> Results

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exe.cpp</a>
