
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/regressor.h"
#include "cubic_engine/optimization/serial_batch_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"

#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include <iostream>

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

    typedef MSEFunction<RealVectorPolynomialFunction,
            DynMat<real_t>,
            DynVec<uint_t>> error_t;

    try{

        auto dataset = kernel::load_car_plant_dataset();

        // The regressor to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1
        // set initial weights to 0
        LinearRegression regressor({0.0, 0.0});

        GDControl control(10000, kernel::KernelConsts::tolerance(),
                          GDControl::DEFAULT_LEARNING_RATE);

        Gd<error_t> gd(control);

        auto result = regressor.train(dataset.first, dataset.second, gd);
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

