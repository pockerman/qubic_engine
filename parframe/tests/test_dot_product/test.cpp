#include "parframe/models/linear_algebra/dot_product.h"
#include "parframe/executors/thread_pool.h"
#include "parframe/base/types.h"
#include "parframe/data_structs/partitioned_object.h"
#include "parframe/data_structs/range_1d.h"
#include "parframe/partitioners/array_partitioner.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TestDotProduct, TestDotProductInvalidVectorSizes) {

    /***
       * Test Scenario:   The application attempts to get the dot product of two vectors of unequal size
       * Expected Output:	ParFrameLib throws a std::runtime_error
     **/

    /// TODO: Is there a better way to do this?
    try{

        using Vector = parframe::PartitionedType<kernel::DynVec<kernel::real_t>>;
        Vector v1(10);
        Vector v2(20);
        parframe::ThreadPool pool(1);

        kernel::DotProduct<Vector, kernel::real_t> product(v1, v2);
        product.execute(pool);
    }
    catch(std::runtime_error& e){
        std::string except_msg(e.what());
        ASSERT_EQ(except_msg, "Invalid vector sizes");
    }
}


TEST(TestDotProduct, TestDotProductReturnedResult) {

    /***
       * Test Scenario:   The application attempts to get the dot product of two vectors of the same size
       * Expected Output:	ParFrameLib should compute the dot product of two vectors and return the result
     **/

    /// TODO: Is there a better way to do this?
    try{

        using Vector = parframe::PartitionedType<kernel::DynVec<kernel::real_t>>;
        Vector v1(10, 1.0);
        Vector v2(10, 1.0);
        parframe::ThreadPool pool(1);
        std::vector<parframe::range1d<parframe::uint_t>> partitions;
        parframe::partition_range(0, v1.size(), partitions, pool.get_n_threads() );

        v1.set_partitions(partitions);
        v2.set_partitions(partitions);

        kernel::DotProduct<Vector, kernel::real_t> product(v1, v2);
        product.execute(pool);

        auto& rslt = product.get();
        EXPECT_EQ(rslt.is_result_valid(), true);
        EXPECT_EQ(*rslt.get().first, 10.0);

    }
    catch(...){

        ASSERT_EQ(1, 2)<<"Test TestDotProduct::TestDotProductReturnedResult failed with unknown exception";

    }
}




