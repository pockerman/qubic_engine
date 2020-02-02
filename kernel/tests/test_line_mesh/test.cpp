#include "kernel/base/types.h"
#include "kernel/discretization/line_mesh.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TestLineMesh, TestDefaultInitialization) {

    /***
       * Test Scenario:    The application attempts to initialize a default contructed LineMesh
       * Expected Output:  Object should be constructed
     **/

    using kernel::numerics::LineMesh;
    LineMesh mesh;
    ASSERT_EQ(mesh.n_nodes(), 0);
    ASSERT_EQ(mesh.n_elements(), 0);
}


TEST(TestLineMesh, TestInsertElement) {

    /***
       * Test Scenario:   The application attempts to insert a valid element into LineMesh
       * Expected Output: Element should be inserted
     **/

    using kernel::numerics::LineMesh;
    LineMesh mesh;
    ASSERT_EQ(mesh.n_nodes(), 0);
    ASSERT_EQ(mesh.n_elements(), 0);
    auto ptr = mesh.add_element();

    ASSERT_EQ(mesh.n_nodes(), 0);
    ASSERT_EQ(mesh.n_elements(), 1);
    ASSERT_TRUE(ptr);

}

TEST(TestLineMesh, TestReInsertDof) {

    /***
       * Test Scenario:   The application attempts to insert a  DoF ithat already exists into DoFObject
       * Expected Output: std::logic_error should be thrown
     **/


}




