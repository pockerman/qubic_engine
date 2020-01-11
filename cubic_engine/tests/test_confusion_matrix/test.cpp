#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/maths/confusion_matrix.h"

#include <vector>
#include <utility>

#include <gtest/gtest.h>

namespace test_data
{
using uint_t = cengine::uint_t;
using real_t = cengine::real_t;
using cengine::ConfusionMatrix;
}

/**
 * @brief TEST TestKnnClassifier
 * Test Scenario:  Application instantiates a ConfusionMatrix where all predictions agree
 *                 with the actual classes
 * Expected Output: Correct construction of the confusion matrix
 */

TEST(TestConfusionMatrix, TestCorrectConstruction_I) {

    using namespace test_data;

    // create a 2x2 confusion matrix
    // 10 samples the first half items have class 0
    // the remaining half have class 1

    std::vector<uint_t> actual(10, 0);
    std::vector<uint_t> predicted(10, 0);
    uint_t middle = actual.size() >> 1;

    for(uint_t t=middle; t< actual.size(); ++t){
        actual[t] = 1;
        predicted[t] = 1;
    }

    ConfusionMatrix matrix(actual, predicted, 2);

    ASSERT_EQ(matrix.get_class_counts(0), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 0\n";
    ASSERT_EQ(matrix.get_class_counts(1), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 1\n";

    // nothing should be misclassified
    ASSERT_EQ(matrix.get_class_incorrect_counts(0), 0)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 0\n";
    ASSERT_EQ(matrix.get_class_incorrect_counts(1), 0)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 1\n";


}


/**
 * @brief TEST TestKnnClassifier
 * Test Scenario:  Application instantiates a ConfusionMatrix where not all predictions agree
 *                 with the actual classes
 * Expected Output: Correct construction of the confusion matrix
 */

TEST(TestConfusionMatrix, TestCorrectConstruction_II) {

    using namespace test_data;

    // create a 2x2 confusion matrix
    // 10 samples the first half items have class 0
    // the remaining half have class 1

    std::vector<uint_t> actual(20, 0);
    std::vector<uint_t> predicted(20, 0);
    uint_t middle = actual.size() >> 1;

    // items [5, 10) are misclassified
    for(uint_t t = 5; t< middle; ++t){
       predicted[t] = 1;
    }


    for(uint_t t = middle; t< 15; ++t){
        actual[t] = 1;
        predicted[t] = 0;
    }

    for(uint_t t = 15; t< actual.size(); ++t){
        actual[t] = 1;
        predicted[t] = 1;
    }

    ConfusionMatrix matrix(actual, predicted, 2);

    ASSERT_EQ(matrix.get_class_counts(0), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 0\n";
    ASSERT_EQ(matrix.get_class_counts(1), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 1\n";

    // nothing should be misclassified
    ASSERT_EQ(matrix.get_class_incorrect_counts(0), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 0\n";
    ASSERT_EQ(matrix.get_class_incorrect_counts(1), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 1\n";

}


/**
 * @brief TEST TestKnnClassifier
 * Test Scenario:  Application instantiates a ConfusionMatrix where all predictions agree
 *                 with the actual classes three different classes
 * Expected Output: Correct construction of the confusion matrix
 */

TEST(TestConfusionMatrix, TestCorrectConstruction_III) {

    using namespace test_data;

    // create a 2x2 confusion matrix
    // 10 samples the first half items have class 0
    // the remaining half have class 1

    std::vector<uint_t> actual(10, 0);
    std::vector<uint_t> predicted(10, 0);
    uint_t middle = actual.size() >> 1;

    // items [5, 10) are misclassified
    for(uint_t t = 3; t< middle; ++t){
       predicted[t] = 1;
       actual[t] = 1;
    }


    for(uint_t t = middle; t< actual.size(); ++t){
        actual[t] = 2;
        predicted[t] = 2;
    }

    ConfusionMatrix matrix(actual, predicted, 3);

    ASSERT_EQ(matrix.get_class_counts(0), 3)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 0\n";
    ASSERT_EQ(matrix.get_class_counts(1), 2)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 1\n";
    ASSERT_EQ(matrix.get_class_counts(2), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 5\n";

    // nothing should be misclassified
    ASSERT_EQ(matrix.get_class_incorrect_counts(0), 0)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 0\n";
    ASSERT_EQ(matrix.get_class_incorrect_counts(1), 0)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 1\n";
    ASSERT_EQ(matrix.get_class_incorrect_counts(2), 0)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 2\n";

}


/**
 * @brief TEST TestKnnClassifier
 * Test Scenario:  Application instantiates a ConfusionMatrix where not all predictions agree
 *                 with the actual classes three classes
 * Expected Output: Correct construction of the confusion matrix
 */

TEST(TestConfusionMatrix, TestCorrectConstruction_IV) {

    using namespace test_data;

    // create a 2x2 confusion matrix
    // 10 samples the first half items have class 0
    // the remaining half have class 1

    std::vector<uint_t> actual(20, 0);
    std::vector<uint_t> predicted(20, 0);
    uint_t middle = actual.size() >> 1;

    // items [5, 10) are misclassified
    // class 1 instead of 0
    for(uint_t t = 5; t< middle; ++t){
       predicted[t] = 1;
    }

    // items [10, 15) are misclassified
    // class 1 instead of 2
    for(uint_t t = middle; t< 15; ++t){
        actual[t] = 2;
        predicted[t] = 1;
    }

    for(uint_t t = 15; t< actual.size(); ++t){
        actual[t] = 1;
        predicted[t] = 1;
    }

    ConfusionMatrix matrix(actual, predicted, 3);

    ASSERT_EQ(matrix.get_class_counts(0), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 0\n";
    ASSERT_EQ(matrix.get_class_counts(1), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 1\n";
    ASSERT_EQ(matrix.get_class_counts(2), 0)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect class count for class 1\n";

    // nothing should be misclassified
    ASSERT_EQ(matrix.get_class_incorrect_counts(0), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 0\n";
    ASSERT_EQ(matrix.get_class_incorrect_counts(1), 0)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 1\n";
    ASSERT_EQ(matrix.get_class_incorrect_counts(2), 5)<<"TestConfusionMatrix::TestCorrectConstruction_I failed with incorrect misclassified class count for class 1\n";

}


