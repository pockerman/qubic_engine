#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "cubic_engine/ml/datasets/data_set_loaders.h"
#include "cubic_engine/ml/supervised_learning/linear_regressor.h"
#include "cubic_engine/ml/supervised_learning/regularizer_type.h"
#include "cubic_engine/ml/optimization/optimizer_type.h"
#include "kernel/numerics/optimization/utils/gd_control.h"
#include "kernel/maths/errorfunctions/error_function_type.h"

#include <vector>
#include <map>
#include <string>
#include <any>

#include <stdexcept>
#include <gtest/gtest.h>

namespace {
using cengine::DynMat;
using cengine::DynVec;
using cengine::real_t;
using cengine::ml::BlazeRegressionDataset;
using cengine::ml::LinearRegressor;
using cengine::ml::RegularizerType;
using cengine::ml::opt::OptimizerType;
using kernel::numerics::opt::GDConfig;
using kernel::ErrorFuncType;

struct TestSetLoader{

    template<typename ColsTp>
    void load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& columns) const;
};

template<typename ColsTp>
void
TestSetLoader::load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& /*columns*/)const{

    cengine::ml::load_car_plant_multi_dataset(mat, labels, 2, false);
}

}


TEST(TestLinearRegressor, Constructor) {

    try{

        LinearRegressor regressor(2, false, RegularizerType::INVALID_TYPE);
    }
    catch(...){

        FAIL()<<"Could not build LinearRegressor";
    }
}

TEST(TestLinearRegressor, Empty_Options) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LinearRegressor regressor(2, false, RegularizerType::INVALID_TYPE);

        // attempt to fit with an empty dataset
        EXPECT_DEATH(regressor.fit(dataset, std::map<std::string, std::any>()), "Options are empty");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLinearRegressor, Empty_Solver_Type) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LinearRegressor regressor(2, false, RegularizerType::INVALID_TYPE);

        std::map<std::string, std::any> options;
        options["error_function_type"] = nullptr;
        options["solver_options"] = nullptr;

        // attempt to fit with an empty dataset
        EXPECT_DEATH(regressor.fit(dataset, options), "Solver was not specified");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLinearRegressor, Empty_Solver_Options) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LinearRegressor regressor(2, false, RegularizerType::INVALID_TYPE);

        std::map<std::string, std::any> options;
        options["solver_type"] = nullptr;
        options["error_function_type"] = nullptr;

        // attempt to fit with an empty dataset
        EXPECT_DEATH(regressor.fit(dataset, options), "Solver options not specified");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLinearRegressor, Empty_Error_Metric) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LinearRegressor regressor(2, false, RegularizerType::INVALID_TYPE);

        std::map<std::string, std::any> options;
        options["solver_type"] = nullptr;
        options["solver_options"] = nullptr;

        // attempt to fit with an empty dataset
        EXPECT_DEATH(regressor.fit(dataset, options), "Error metric was not specified");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLinearRegressor, End_To_End) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LinearRegressor regressor(2, false, RegularizerType::INVALID_TYPE);

        std::map<std::string, std::any> options;
        options["solver_type"] = OptimizerType::GD;
        options["error_function_type"] = ErrorFuncType::MSE;

        std::map<std::string, std::any> solver_opts;
        options["solver_options"] = solver_opts;

        // attempt to fit with an empty dataset
        regressor.fit(dataset, options);
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}
















