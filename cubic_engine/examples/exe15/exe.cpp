
#include "cubic_engine/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/pytorch_linear_regressor.h"
#include "cubic_engine/optimization/pytorch_stochastic_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"

//#include "kernel/maths/functions/real_vector_polynomial.h"
//#include "kernel/maths/errorfunctions/mse_function.h"
//#include "kernel/utilities/data_set_loaders.h"


#include <torch/torch.h>

#include <iostream>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::GDControl;
    using cengine::pytorch::PYT_LinearRegressor;
    using cengine::pytorch::PYT_StochasticGD;
    using cengine::Null;
   // using kernel::RealVectorPolynomialFunction;
   // using kernel::MSEFunction;

    //typedef torch::nn::functional::MSEloss error_t;


    try{

        /// load the dataset
        /*typedef torch::nn::MSELoss error_t;
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
        regressor.train(x_train, y_train, sgd);*/


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

