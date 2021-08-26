#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/knn_classifier.h"
#include "cubic_engine/ml/instance_learning/knn_control.h"
#include "kernel/maths/functions/real_vector_polynomial.h"
#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"

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
using cengine::ml::KnnClassifier;
using cengine::ml::KnnControl;

struct TestErrorMetric
{

    template<typename Vec1, typename Vec2>
    real_t operator()(const Vec1&, const Vec2&){return 0.0;}

};


}


TEST(KnnClassifier, Constructor) {

    try{

        KnnControl control(1);
        KnnClassifier<BlazeRegressionDataset, TestErrorMetric> knn(control);
    }
    catch(...){

        FAIL()<<"Could not build KnnClassifier";
    }
}

/*TEST(TestSSELoss, Error_At) {

    try{

        std::vector<real_t> coeffs(2, 1.0);
        PolynomialFunction model(coeffs);
        SSELoss<PolynomialFunction, BlazeRegressionDataset> loss(model);

        DynVec<real_t> p(2, 1.0);
        real_t val = 0.5;

        auto err = loss.error_at(p, val);

        ASSERT_DOUBLE_EQ(err, 1.5*1.5);
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}

TEST(TestSSELoss, Param_Gradient_At) {

    try{

        std::vector<real_t> coeffs(2, 1.0);
        PolynomialFunction model(coeffs);
        SSELoss<PolynomialFunction, BlazeRegressionDataset> loss(model);

        DynVec<real_t> p(2, 1.0);
        real_t val = 0.5;

        auto err = loss.error_at(p, val);

        ASSERT_DOUBLE_EQ(err, 1.5*1.5);

        auto grads = loss.param_gradient_at(p, val);

        ASSERT_EQ(grads.size(), 2);
        ASSERT_DOUBLE_EQ(grads[0], -2.0 * err * p[0]);
        ASSERT_DOUBLE_EQ(grads[1], -2.0 * err * p[1]);
    }
    catch(...){
        FAIL()<<"A non expected exception was thrown";
    }
}


TEST(TestMSELoss, Constructor) {

    try{

        PolynomialFunction model;
        MSELoss<PolynomialFunction, BlazeRegressionDataset> loss(model);
    }
    catch(...){

        FAIL()<<"Could not build SSELoss";
    }
}
*/
















