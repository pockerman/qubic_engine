#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "cubic_engine/ml/datasets/data_set_loaders.h"
#include "cubic_engine/ml/supervised_learning/logistic_regression.h"


#include "kernel/numerics/optimization/gd_control.h"
#include "kernel/maths/errorfunctions/error_function_type.h"
#include "kernel/numerics/optimization/optimizer_type.h"
#include "kernel/utilities/algorithm_info.h"

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
using cengine::ml::LogisticRegression;
using kernel::numerics::opt::OptimizerType;
using kernel::numerics::opt::GDConfig;
using kernel::ErrorFuncType;
using kernel::AlgInfo;

struct TestSetLoader{

    template<typename ColsTp>
    void load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& columns) const;
};

template<typename ColsTp>
void
TestSetLoader::load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& /*columns*/)const{

    cengine::ml::load_car_plant_multi_dataset(mat, labels, 2, false);
}

struct TestSolver{

    typedef AlgInfo output_t;

    template<typename MatTyp, typename LabelTp, typename FunTp>
    output_t solve(MatTyp& /*mat*/, LabelTp& /*labels*/, FunTp& /*columns*/) const{return output_t();}
};

}


TEST(TestLogisticRegression, Constructor) {

    try{

        LogisticRegression regressor(2, false);
    }
    catch(...){

        FAIL()<<"Could not build LogisticRegressor";
    }
}

TEST(TestLogisticRegression, Empty_Options) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LogisticRegression regressor(2, false);

        // attempt to fit with an empty dataset
        TestSolver solver;
        EXPECT_DEATH(regressor.fit(dataset, solver, std::map<std::string, std::any>()), "Options are empty");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLogisticRegression, DISABLED_Empty_Solver_Type) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LogisticRegression regressor(2, false);

        std::map<std::string, std::any> options;
        options["error_function_type"] = nullptr;
        options["solver_options"] = nullptr;

        // attempt to fit with an empty dataset
         TestSolver solver;
        EXPECT_DEATH(regressor.fit(dataset, solver, options), "Solver was not specified");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLogisticRegression, DISABLED_Empty_Solver_Options) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LogisticRegression regressor(2, false);

        std::map<std::string, std::any> options;
        options["solver_type"] = nullptr;
        options["error_function_type"] = nullptr;

        // attempt to fit with an empty dataset
         TestSolver solver;
        EXPECT_DEATH(regressor.fit(dataset, solver, options), "Solver options not specified");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLogisticRegression, Empty_Error_Metric) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LogisticRegression regressor(2, false);

        std::map<std::string, std::any> options;
        options["solver_type"] = nullptr;
        options["solver_options"] = nullptr;

        // attempt to fit with an empty dataset
         TestSolver solver;
        EXPECT_DEATH(regressor.fit(dataset, solver, options), "Error metric was not specified");
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLogisticRegression, End_To_End) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LogisticRegression regressor(2, false);

        std::map<std::string, std::any> options;
        options["solver_type"] = OptimizerType::GD;
        options["error_function_type"] = ErrorFuncType::MSE;

        std::map<std::string, std::any> solver_opts;
        options["solver_options"] = solver_opts;

        // attempt to fit with an empty dataset
         TestSolver solver;
        regressor.fit(dataset, solver, options);
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestLogisticRegression, Predict_Valid_Shape)
{

    try{

        // use two features no intercept term
        LogisticRegression regressor(2, false);

        DynVec<real_t> point(2, 0.0);
        auto value = regressor.predict_one(point);

    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }

}

TEST(TestLogisticRegression, Predict_Invalid_Shape_1)
{

    try{

        // use two features no intercept term
        LogisticRegression regressor(2, false);

        DynVec<real_t> point(3, 0.0);
        EXPECT_DEATH(regressor.predict_one(point), "Invalid data point shape");

    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }

}
















