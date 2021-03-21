#include "kernel/base/config.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/base/types.h"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

namespace{

}

TEST(TrilinosEpetraMatrix, GetRowEntries) {

#ifdef USE_TRILINOS

    /// Test Scenario:   Initialize an n x n matrix and extract a row
    /// Expected Output: An n-sized vector should be returned

    using kernel::numerics::TrilinosEpetraMatrix;
    using kernel::uint_t;
    using kernel::real_t;

    TrilinosEpetraMatrix matrix;
    matrix.init(5, 5, 0);

    TrilinosEpetraMatrix::row_entries_t row_entries = {0., 1., 2., 3., 4.};

    matrix.set_row_entries(2, row_entries);
    matrix.fill_completed();

    TrilinosEpetraMatrix::row_entries_t row = matrix.get_row_entries(2);

    ASSERT_EQ(row.size(), 5);

    for(uint_t c=0; c<row.size(); ++c){
        ASSERT_DOUBLE_EQ(matrix.entry(2, c), static_cast<real_t>(c));
    }
#endif
}

TEST(TrilinosEpetraMatrix, ComputeTranspose) {

#ifdef USE_TRILINOS

     /// Test Scenario:   Initialize an n x m matrix and compute its transpose
     /// Expected Output: An m x n matrix should be created

    using kernel::numerics::TrilinosEpetraMatrix;
    using kernel::uint_t;
    using kernel::real_t;

    TrilinosEpetraMatrix matrix(2, 0);

    TrilinosEpetraMatrix::row_entries_t row_entries0 = {0., 1.};
    TrilinosEpetraMatrix::row_entries_t row_entries1 = {3., 4.};

    matrix.set_row_entries(0, row_entries0);
    matrix.set_row_entries(1, row_entries1);
    matrix.fill_completed();


    ASSERT_EQ(matrix.m(), 2);
    ASSERT_EQ(matrix.n(), 2);

    TrilinosEpetraMatrix transpose(matrix.m(), 0);
    matrix.compute_transpose(transpose);

    transpose.print(std::cout);

    ASSERT_EQ(transpose.m(), 2);
    ASSERT_EQ(transpose.n(), 2);

    ASSERT_DOUBLE_EQ(transpose.entry(0, 0), 0.);
    ASSERT_DOUBLE_EQ(transpose.entry(0, 1), 3.);
    ASSERT_DOUBLE_EQ(transpose.entry(1, 0), 1.);
    ASSERT_DOUBLE_EQ(transpose.entry(1, 1), 4.);
#endif
}






