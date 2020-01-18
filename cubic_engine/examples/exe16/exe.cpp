
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/regressor.h"
#include "cubic_engine/optimization/serial_batch_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/maths/functions/lasso_function.h"
#include "cubic_engine/maths/functions/ridge_function.h"
#include "cubic_engine/maths/functions/elastic_net_function.h"


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
    using cengine::LassoFunction;
    using cengine::RidgeFunction;
    using cengine::ElasticNetFunction;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;

    try{

        auto dataset = kernel::load_x_y_sinuisoid_data_set();

        typedef RealVectorPolynomialFunction hypothesis_t;
        typedef LassoFunction<hypothesis_t, DynMat<real_t>, DynVec<real_t>> lasso_t;
        typedef RidgeFunction<hypothesis_t, DynMat<real_t>, DynVec<real_t>> ridge_t;
        typedef ElasticNetFunction<hypothesis_t, DynMat<real_t>, DynVec<real_t>> elastic_net_t;

        {
            std::cout<<"Using Lasso Regularizer"<<std::endl;

            // the classifier to use. use a hypothesis of the form
            // f = w_0 + w_1*x_1
            // set initial weights to 0
            LinearRegression regressor({0.0, 0.0});

            /// lasso regulaizer
            lasso_t lasso(regressor.get_model(), 0.0001, 1,
                          regressor.get_model().n_coeffs());

            GDControl control(10000, kernel::KernelConsts::tolerance(),
                                       GDControl::DEFAULT_LEARNING_RATE);
            control.show_iterations = false;

            typedef MSEFunction<hypothesis_t, DynMat<real_t>, DynVec<uint_t>, lasso_t> error_t;
            Gd<error_t> gd(control);

            auto result = regressor.train(dataset.first, dataset.second, gd, lasso);
            std::cout<<result<<std::endl;

        }

        {
            std::cout<<"Using Ridge Regularizer"<<std::endl;

            // the classifier to use. use a hypothesis of the form
            // f = w_0 + w_1*x_1
            // set initial weights to 0
            LinearRegression regressor({0.0, 0.0});

            ridge_t ridge(regressor.get_model(), 0.001, 1,
                          regressor.get_model().n_coeffs());

            GDControl control(10000, kernel::KernelConsts::tolerance(),
                              GDControl::DEFAULT_LEARNING_RATE);
            control.show_iterations = false;

            // the error function to to use for measuring the error
            typedef MSEFunction<hypothesis_t, DynMat<real_t>, DynVec<uint_t>, ridge_t> error_t;
            Gd<error_t> gd(control);

            auto result = regressor.train(dataset.first, dataset.second, gd, ridge);
            std::cout<<result<<std::endl;

        }

        {
            std::cout<<"Using ElasticNet Regularizer"<<std::endl;

            // the classifier to use. use a hypothesis of the form
            // f = w_0 + w_1*x_1
            // set initial weights to 0
            LinearRegression regressor({0.0, 0.0});

             elastic_net_t elastic_net(regressor.get_model(), 0.001, 0.0001,
                                       1, regressor.get_model().n_coeffs());

            GDControl control(10000, kernel::KernelConsts::tolerance(),
                                       GDControl::DEFAULT_LEARNING_RATE);
            control.show_iterations = false;

            // the error function to to use for measuring the error
            typedef MSEFunction<hypothesis_t, DynMat<real_t>, DynVec<uint_t>, elastic_net_t> error_t;
            Gd<error_t> gd(control);

            auto result = regressor.train(dataset.first, dataset.second, gd, elastic_net);
            std::cout<<result<<std::endl;

        }
    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

