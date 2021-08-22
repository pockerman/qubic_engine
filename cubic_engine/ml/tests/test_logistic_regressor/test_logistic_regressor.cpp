#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "cubic_engine/ml/datasets/data_set_loaders.h"
#include "cubic_engine/ml/supervised_learning/logistic_regression.h"
#include "kernel/utilities/iterative_algorithm_result.h"

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
using cengine::uint_t;
using cengine::ml::BlazeRegressionDataset;
using cengine::ml::LogisticRegression;
using kernel::IterativeAlgorithmResult;

struct TestSetLoader{

    template<typename ColsTp>
    void load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& columns) const;
};

template<typename ColsTp>
void
TestSetLoader::load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& /*columns*/)const{

    cengine::ml::load_car_plant_multi_dataset(mat, labels, 2, false);
}


struct TestFunction
{

    template<typename RowTp, typename T>
    real_t error_at(const RowTp&, const T&)const{return 0.0;}

    template<typename RowTp, typename T>
    DynVec<real_t> param_gradient_at(const RowTp&, const T& )const{return DynVec<real_t>();}

    DynVec<real_t> parameters()const{return DynVec<real_t>();}

    uint_t n_parameters()const{return 1;}

    template<typename Data>
    real_t evaluate(const Data&){return 0.0;}

    template<typename Data>
    DynVec<real_t> params_gradients(const Data&)const{return DynVec<real_t>();}

    template<typename Container>
    void update_parameters(const Container&){}

    template<typename Model>
    TestFunction(Model&){}

};

struct TestSolver{

    typedef IterativeAlgorithmResult output_t;
    typedef TestFunction function_t ;

    template<typename MatTyp, typename FunTp>
    output_t solve(MatTyp& /*mat*/, FunTp& /*columns*/) const{return output_t();}
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


TEST(TestLogisticRegression, End_To_End) {

    try{

        BlazeRegressionDataset dataset;
        dataset.load_from_data(DynMat<real_t>(1, 2, 0.0), DynVec<real_t>(1, 0.0));
        LogisticRegression regressor(2, false);

        std::map<std::string, std::any> options;

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
















