#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/serial_knn.h"
#include "cubic_engine/ml/instance_learning/knn_classification_policy.h"
#include "kernel/maths/lp_metric.h"

#include <vector>
#include <gtest/gtest.h>

namespace test_data
{
using uint_t = cengine::uint_t;
using real_t = cengine::real_t;
using Mat = cengine::DynMat<real_t>;
using VecInt = cengine::DynVec<uint_t>;
using VecDouble = cengine::DynVec<real_t>;

void create_data_set(Mat& mat, VecInt& vec ){

    mat(0,0) = 1.0;
    mat(0,1) = 3.0;
    mat(1,0) = 1.5;
    mat(1,1) = 2.0;
    mat(2,0) = 2.0;
    mat(2,1) = 1.0;
    mat(3,0) = 2.5;
    mat(3,1) = 4.0;
    mat(4,0) = 3.0;
    mat(4,1) = 1.5;
    mat(5,0) = 3.5;
    mat(5,1) = 2.5;
    mat(6,0) = 5.0;
    mat(6,1) = 5.0;
    mat(7,0) = 5.5;
    mat(7,1) = 4.0;
    mat(8,0) = 6.0;
    mat(8,1) = 6.0;
    mat(9,0) = 6.5;
    mat(9,1) = 4.5;
    mat(10,0) = 7.0;
    mat(10,1) = 1.5;
    mat(11,0) = 8.0;
    mat(11,1) = 2.5;

    vec[0]=vec[1]=vec[2]=vec[3]=vec[4]=vec[5]=0;
    vec[6]=vec[7]=vec[8]=vec[9]=vec[10]=vec[11]=1;
}

}

/**
 * @brief TEST TestKnnClassifier
 * Test Scenario:  Application instantiates a KNN-classifier
 * Expected Output: Correct class should be predicted
 */

TEST(TestKnnClassifier, TestCorrectClass) {

    using namespace test_data;
    Mat matrix(12, 2, 0.0);
    VecInt labels(12, 0);

    create_data_set(matrix, labels);

    cengine::KnnInput input;
    input.k = 2;

    cengine::Knn<Mat, VecInt, kernel::LpMetric<2>, cengine::KnnClassificationPolicy> classifier(input);

    VecDouble point(2);
    point[0] = 3.1;
    point[1] = 2.0;
    auto reslt = classifier.predict(point);

    ASSERT_EQ(reslt, 0)<<"TestKnnClassifier::TestCorrectClass failed with incorrect class\n";

}
