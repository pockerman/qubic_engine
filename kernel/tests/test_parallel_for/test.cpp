#include "kernel/parallel/parallel_algos/parallel_for.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/base/types.h"
#include <vector>
#include <gtest/gtest.h>

namespace{



}

TEST(TestParallelFor, RunWithEmptyRange) {

    using kernel::uint_t;
    using kernel::ThreadPool;
    using kernel::range1d;

    /***
     * Test Scenario:   The application attempts to execute parallel_for on an empty Range 
     * Expected Output:	parallel_for simply returns silently. If warnings are one a message is printed
     **/

    /// TODO: Is there a better way to do this?
    try{

        // this is the executor
        ThreadPool pool(4);
        range1d<uint_t> range;

        // dummy body
        auto body = [](int){return false;};
        kernel::parallel_for(range, body, pool);

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown");
    }
}




