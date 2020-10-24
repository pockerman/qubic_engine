#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/regressor.h"
#include "cubic_engine/ml/supervised_learning/ordinary_least_squares.h"
#include "kernel/maths/optimization/serial_gradient_descent.h"
#include "kernel/maths/optimization/utils/gd_control.h"
#include "kernel/utilities/data_set_loaders.h"


#include <iostream>

int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using kernel::maths::opt::GDConfig;
    using kernel::maths::opt::Gd;
    using cengine::LinearRegression;
    using cengine::ml::OrdinaryLeastSquares;

    try{

        // load the data set
        auto dataset = kernel::load_car_plant_dataset();

        // linear model trained with OLS
        OrdinaryLeastSquares ols(1, true);

        // train the model
        ols.train(dataset.first, dataset.second);

        std::cout<<"Intercept: "<<ols.get_interception()<<" slope: "<<ols.coeff(0)<<std::endl;

        // The regressor to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1
        // set initial weights to 0
        LinearRegression regressor({0.0, 0.0});

        // configuration for GD
        GDConfig control(10000, 1.0e-7, GDConfig::DEFAULT_LEARNING_RATE);

        Gd gd(control);

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

