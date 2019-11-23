#include "kernel/base/types.h"
#include "kernel/maths/matrix_utilities.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}

/***
   * Test Scenario:   The application attempts to exchange two rows of a given matrix
   * Expected Output: Rows should be exchanged
 **/
TEST(TestMatrixUtilities, TestExcahngeMatrixRows) {

   using uint_t = kernel::uint_t;
   using DynMat = kernel::DynMat<uint_t>;

   uint_t data[3][3] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
   DynMat matrix(3, 3);

   matrix = data;
   kernel::exchange_rows(matrix, 0, 2);

   ASSERT_EQ(matrix(0, 0), 3);
   ASSERT_EQ(matrix(0, 1), 3);
   ASSERT_EQ(matrix(0, 2), 3);

   ASSERT_EQ(matrix(2, 0), 1);
   ASSERT_EQ(matrix(2, 1), 1);
   ASSERT_EQ(matrix(2, 2), 1);

}

/***
   * Test Scenario:    The application attempts to shuffle the rows of a given matrix
   * Expected Output:  Rows should be shuffled successfully
 **/
TEST(TestMatrixUtilities, TestShuffleRows) {

    using uint_t = kernel::uint_t;
    using DynMat = kernel::DynMat<uint_t>;

    uint_t data[3][3] = {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}};
    DynMat matrix(3, 3);

    matrix = data;
    auto set = kernel::shuffle_matrix_rows(matrix);

    ASSERT_FALSE(set.empty())<<"No rows were shuffled\n";
}




