#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/maths/trilinos_epetra_multivector.h"

#include "kernel/base/types.h"
#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TrilinosEpetraMultiVector, InitNxN) {

     /// Test Scenario:   Initialize an n x n multi-vector
     /// Expected Output: An n x n multi-vector should be created

    kernel::maths::algebra::TrilinosEpetraMultiVector vector;
    vector.init(10, true);

    ASSERT_EQ(vector.size(), 10);
    ASSERT_EQ(vector.n_vectors(), 10);
}

TEST(TrilinosEpetraMultiVector, InitNxM) {

     /// Test Scenario:   Initialize an n x m multi-vector
     /// Expected Output: An n x m multi-vector should be created

    kernel::maths::algebra::TrilinosEpetraMultiVector vector;
    vector.init(10, 5, true);

    ASSERT_EQ(vector.size(), 5);
    ASSERT_EQ(vector.n_vectors(), 10);
}

TEST(TrilinosEpetraMultiVector, Set) {

     /// Test Scenario:   Initialize an 1 x 5 multi-vector and
     ///                  set all entries to 10
     /// Expected Output: An 1 x 5 multi-vector should be created
     ///                  with entries equal to 10

    kernel::maths::algebra::TrilinosEpetraMultiVector vector;
    vector.init(1, 5, true);

    vector.set_entry(0, 0, 10.);
    ASSERT_DOUBLE_EQ(vector.get(0, 0), 10.);

    vector.set_entry(0, 2, 5.);
    ASSERT_DOUBLE_EQ(vector.get(0, 2), 5.);

}

TEST(TrilinosEpetraMultiVector, Add) {

     /// Test Scenario:   Initialize an 1 x 5 multi-vector and
     ///                  add 1 to the third entry
     /// Expected Output: An 1 x 5 multi-vector should be created
     ///                  with entry 3 equal to 2

    kernel::maths::algebra::TrilinosEpetraMultiVector vector;
    vector.init(1, 5, true);

    vector.set_entry(0, 3, 1.);
    ASSERT_DOUBLE_EQ(vector.get(0, 3), 1.);

    vector.add_entry(0, 3, 1.);
    ASSERT_DOUBLE_EQ(vector.get(0, 3), 2.);

}

#endif




