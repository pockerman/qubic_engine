#include "kernel/utilities/data_set_loaders.h"
#include "kernel/base/types.h"
#include "kernel/base/exceptions.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}


/***
 * Test Scenario:   The application attempts to load the reduced iris data set
 * Expected Output:	Data set should be loaded
 **/

TEST(TestDataSetLoaders, LoadReducedIrisSet) {


    /// TODO: Is there a better way to do this?
    try{

        auto data = kernel::load_reduced_iris_data_set();
        ASSERT_EQ(data.first.rows(), 100);
        ASSERT_EQ(data.first.columns(), 5);
        ASSERT_EQ(data.second.size(), data.first.rows());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

/***
 * Test Scenario:   The application attempts to load the x_y sinuisoid iris data set
 * Expected Output:	Data set should be loaded
 **/
TEST(TestDataSetLoaders, LoadXYSinuisoidDataSet) {


    /// TODO: Is there a better way to do this?
    try{

        auto data = kernel::load_x_y_sinuisoid_data_set();
        ASSERT_EQ(data.first.rows(), 20);
        ASSERT_EQ(data.first.columns(), 2);
        ASSERT_EQ(data.second.size(), data.first.rows());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

/***
 * Test Scenario:   The application attempts to load the x_y sinuisoid iris data set
 * Expected Output:	Data set should be loaded
 **/
TEST(TestDataSetLoaders, LoadXYSinuisoidDataSetNoOnes) {


    /// TODO: Is there a better way to do this?
    try{

        auto data = kernel::load_x_y_sinuisoid_data_set(false);
        ASSERT_EQ(data.first.rows(), 20);
        ASSERT_EQ(data.first.columns(), 1);
        ASSERT_EQ(data.second.size(), data.first.rows());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}








