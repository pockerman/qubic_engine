
#include "cubic_engine/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/pytorch_linear_regressor.h"

#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/maths/functions/sigmoid_function.h"
#include "kernel/utilities/data_set_loaders.h"
#include "kernel/parallel/threading/thread_pool.h"

#include <iostream>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::pytorch::PYLinearRegressor;
    using cengine::Null;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;
    using kernel::SigmoidFunction;
    using kernel::ThreadPool;
    using kernel::PartitionedType;

    try{

        /// load the dataset
        auto dataset = kernel::load_reduced_iris_data_set();

        // the classifier to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1
        // set initial weights to 0. The bias term is included
        // by default
        std::vector<real_t> weights(1, 0.0);
        PYLinearRegressor regressor(weights);

        // let's see the weights
        regressor.print(std::cout);

        // let's train the model


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

