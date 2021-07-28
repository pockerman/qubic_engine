#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/linear_regressor.h"
#include "cubic_engine/ml/supervised_learning/regularizer_type.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "kernel/maths/errorfunctions/error_function_type.h"
#include "kernel/numerics/optimization/optimizer_type.h"
#include "kernel/maths/functions/real_vector_polynomial.h"

#include <iostream>
#include <map>
#include <string>
#include <any>

namespace example_12
{

using cengine::uint_t;
using cengine::real_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::ml::BlazeRegressionDataset;
using cengine::ml::LinearRegressor;

struct DataSetLoader
{
    real_t a_;
    real_t b_;

    DataSetLoader(real_t a, real_t b)
        :
          a_(a),
          b_(b)
    {}


    void load(DynMat<real_t>& features, DynVec<real_t>& labels, std::map<std::string, uint_t>& cols)const;
};

void
DataSetLoader::load(DynMat<real_t>& features, DynVec<real_t>& labels, std::map<std::string, uint_t>& cols)const{

    const auto START = 0.0;
    const auto DX = 0.1;
    const auto NUM_POINTS = 100;

    features.resize(NUM_POINTS, 2);
    labels.resize(NUM_POINTS);

    for(uint_t i = 0; i < NUM_POINTS; ++i){

        features(i, 0) = 1.0;
        features(i, 1) = START + i*DX;
        labels[i] = a_ + (START + i*DX)*b_;
    }

    cols["x"] = 0;
}

}

int main(){

    using namespace example_12;

    try{

        // load the data set
        BlazeRegressionDataset dataset;

        DataSetLoader loader(1.0, 2.0);
        dataset.load_from_loader(loader);

        // linear model trained with OLS
        LinearRegressor regressor(1, true, cengine::ml::RegularizerType::NONE);

        // train the model
        std::map<std::string, std::any> options;
        options["solver_type"] = std::any(kernel::numerics::opt::OptimizerType::GD);
        options["error_function_type"] = std::any(kernel::ErrorFuncType::MSE);

        std::map<std::string, std::any> solver_ops;
        solver_ops["max_num_itrs"] = std::any(static_cast<uint_t>(10000));
        solver_ops["tolerance"] = std::any(static_cast<real_t>(1.0e-8));
        solver_ops["learning_rate"] = std::any(static_cast<real_t>(0.01));
        solver_ops["verbose"] = std::any(true);

        options["solver_options"] = std::any(solver_ops);
        regressor.fit(dataset, options);

        std::cout<<"Intercept: "<<regressor.get_interception()<<" slope: "<<regressor.get_parameters()[1]<<std::endl;

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

