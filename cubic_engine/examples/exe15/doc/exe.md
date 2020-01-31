# Example 15: Linear Regression  With ```PyTorch```

## Contents
* [Overview](#overview) 
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="overview"></a> Overview


## <a name="include_files"></a> Include files

```
#include "cubic_engine/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/pytorch_linear_regressor.h"
#include "cubic_engine/optimization/pytorch_stochastic_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"

#include <torch/torch.h>

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
    using cengine::pytorch::PYT_LinearRegressor;
    using cengine::pytorch::PYT_StochasticGD;

    try{

        typedef torch::nn::MSELoss error_t;

        /// load the dataset
        auto x_train = torch::randint(0, 10, {15, 1});
        auto y_train = torch::randint(0, 10, {15, 1});

        // the classifier to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1
        // set initial weights to 0. The bias term is included
        // by default
        std::vector<real_t> weights(1, 0.0);
        PYT_LinearRegressor regressor(weights);

        // let's see the weights
        regressor.print(std::cout);

        GDControl control(10000, kernel::KernelConsts::tolerance(),
                          GDControl::DEFAULT_LEARNING_RATE);

        PYT_StochasticGD<error_t> sgd(control);
        regressor.train(x_train, y_train, sgd);

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
#else
#include <iostream>
int main(){
std::cout<<"This example requires PyTorch configuration"<<std::endl;
return 0;
}
#endif
```

## <a name="results"></a> Results

Execution of the code driver should show

```

```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exe.cpp</a>
