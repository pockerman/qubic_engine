#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/loss_functions/sse_loss.h"
#include "cubic_engine/ml/loss_functions/mse_loss.h"
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
using kernel::PolynomialFunction;
using cengine::ml::SSELoss;
using cengine::ml::MSELoss;
using cengine::ml::BlazeRegressionDataset;

/*struct TestSetLoader{

    template<typename ColsTp>
    void load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& columns) const;
};

template<typename ColsTp>
void
TestSetLoader::load(DynMat<real_t>& mat, DynVec<real_t>& labels, ColsTp& )const{

    cengine::ml::load_car_plant_multi_dataset(mat, labels, 2, false);
}

struct TestSolver{

    typedef IterativeAlgorithmResult output_t;

    template<typename MatTyp, typename LabelTp, typename FunTp>
    output_t solve(MatTyp& , LabelTp& , FunTp& ) const{return output_t();}
};*/

}


TEST(TestSSELoss, Constructor) {

    try{

        PolynomialFunction model;
        SSELoss<PolynomialFunction, BlazeRegressionDataset> loss(model);
    }
    catch(...){

        FAIL()<<"Could not build SSELoss";
    }
}

TEST(TestSSELoss, Error_At) {

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
















