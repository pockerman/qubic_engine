#include "kernel/utilities/data_set_loaders.h"
#include "kernel/base/types.h"
#include "kernel/base/exceptions.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}


/***
 * Test Scenario:   The application attempts to execute parallel_for with a  Range that is not partitioned
 * Expected Output:	parallel_for throws InvalidPartitionedObject
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








