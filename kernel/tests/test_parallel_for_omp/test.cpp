#include "kernel/base/config.h"

#ifdef USE_OPENMP

#include "kernel/parallel/threading/task_base.h"
#include "kernel/parallel/parallel_algos/parallel_for.h"
#include "kernel/parallel/threading/openmp_executor.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include "kernel/parallel/utilities/array_partitioner.h"
#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/base/types.h"
#include "kernel/base/exceptions.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

struct DummyTestTask: public kernel::TaskBase
{

public:

    DummyTestTask()
        :
          kernel::TaskBase()
    {}

protected:

    /// \brief Function to overrided by defived classes.
    /// It actually executes the compuational task
    void run(){}

};

}


/***
 * Test Scenario:   The application attempts to execute parallel_for with a  Range that is not partitioned
 * Expected Output:	parallel_for throws InvalidPartitionedObject
 **/

TEST(TestParallelFor, RunWithNoPartitions) {

    using kernel::uint_t;
    using kernel::OMPExecutor;
    using kernel::range1d;
    using kernel::PartitionedType;


    /// TODO: Is there a better way to do this?
    try{

        // this is the executor
        OMPExecutor pool(4);

        // a dummy partitoned ranges
        PartitionedType<std::vector<uint_t>> range;

        // dummy body
        auto body = [](int){return false;};
        kernel::parallel_for(range, body, pool, kernel::OMPOptions());

    }
    catch(kernel::InvalidPartitionedObject& e){
        std::string msg = e.what();
        ASSERT_EQ(msg, "The given range does not have partitions");
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}


/***
 * Test Scenario:   The application attempts to execute parallel_for with a  Range that is not correctly partitioned
 * Expected Output:	parallel_for throws InvalidPartitionedObject
 **/

TEST(TestParallelFor, RunWithInvalidNumPartitions) {

    using kernel::uint_t;
    using kernel::OMPExecutor;
    using kernel::range1d;
    using kernel::PartitionedType;

    /// TODO: Is there a better way to do this?
    std::string expected;
    try{

        // this is the executor
        OMPExecutor pool(4);

        // a dummy partitoned ranges
        PartitionedType<std::vector<uint_t>> range;
        std::vector<range1d<uint_t>> partitions(1, range1d<uint_t>());
        range.set_partitions(partitions);

        expected = "Invalid number of partitions: "+std::to_string(range.n_partitions())+" should be: "+std::to_string(pool.n_processing_elements());

        // dummy body
        auto body = [](int){return false;};
        kernel::parallel_for(range, body, pool, kernel::OMPOptions());

    }
    catch(kernel::InvalidPartitionedObject& e){
        std::string msg = e.what();

        ASSERT_EQ(expected, msg);
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}

/***
 * Test Scenario:   The application attempts to execute parallel_for with a  Range and a Body
 * Expected Output:	parallel_for should execute corrctly
 **/


TEST(TestParallelFor, RunWithIdentityBody) {

    using kernel::uint_t;
    using kernel::OMPExecutor;
    using kernel::range1d;
    using kernel::PartitionedType;
    using kernel::ResultHolder;

    /// TODO: Is there a better way to do this?
    try{

        // this is the executor
        OMPExecutor pool(4);

        std::vector<range1d<uint_t>> partitions;
        uint_t n_threads = pool.get_n_threads();
        kernel::partition_range(0, 100, partitions, n_threads);

        // a dummy partitoned ranges
        PartitionedType<std::vector<uint_t>> vector(100, 1);
        vector.set_partitions(partitions);

        // dummy body
        auto body = [](uint_t item){return item;};
        ResultHolder<void> result = kernel::parallel_for(vector, body, pool, kernel::OMPOptions());
        ASSERT_TRUE(result.get().second);

    }
    catch(kernel::InvalidPartitionedObject& e){
        std::string msg = e.what();
        ASSERT_EQ(msg, "The given range does not have partitions");
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}


#endif






