#include "kernel/parallel/parallel_algos/parallel_reduce.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include "kernel/parallel/utilities/array_partitioner.h"
#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/parallel/utilities/identity_reduction.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/base/types.h"
#include "kernel/base/exceptions.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

using kernel::uint_t;
using kernel::ThreadPool;
using kernel::range1d;
using kernel::PartitionedType;
using kernel::IdentityReduce;

}


/***
 * Test Scenario:   The application attempts to execute parallel_for with a  Range that is not partitioned
 * Expected Output:	parallel_for throws InvalidPartitionedObject
 **/

TEST(TestParallelReduce, RunWithNoPartitions) {


    /// TODO: Is there a better way to do this?
    try{

        // this is the executor
        ThreadPool pool(4);

        // a dummy partitoned ranges
        PartitionedType<std::vector<uint_t>> range;

        // dummy reduction
        IdentityReduce<uint_t> reduce;
        kernel::parallel_reduce(range, reduce, pool);

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

TEST(TestParallelReduce, RunWithInvalidNumPartitions) {

    /// TODO: Is there a better way to do this?
    std::string expected;
    try{

        // this is the executor
        ThreadPool pool(4);

        // a dummy partitoned ranges
        PartitionedType<std::vector<uint_t>> range;
        std::vector<range1d<uint_t>> partitions(1, range1d<uint_t>());
        range.set_partitions(partitions);

        expected = "Invalid number of partitions: "+std::to_string(range.n_partitions())+" should be: "+std::to_string(pool.n_processing_elements());

        // dummy reduction
        IdentityReduce<uint_t> reduce;
        kernel::parallel_reduce(range, reduce, pool);

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


TEST(TestParallelReduce, RunWithIdentityBody) {


    /// TODO: Is there a better way to do this?
    try{

        // this is the executor
        ThreadPool pool(4);

        std::vector<range1d<uint_t>> partitions;
        kernel::partition_range(0, 100, partitions, pool.get_n_threads());

        // a dummy partitoned ranges
        PartitionedType<std::vector<uint_t>> vector(100, 0);
        vector.set_partitions(partitions);

        // dummy body
        auto body = [](uint_t item){return item;};
       /*ResultHolder<void> result = kernel::parallel_reduce(vector, body, pool);
        ASSERT_TRUE(result.get().second);*/

    }
    catch(kernel::InvalidPartitionedObject& e){
        std::string msg = e.what();
        ASSERT_EQ(msg, "The given range does not have partitions");
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}






