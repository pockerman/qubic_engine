#include "kernel/base/types.h"
#include "kernel/numerics/dof_object.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TestDoFObject, TestDefaultInitialization) {

    /***
       * Test Scenario:    The application attempts to initialize a default contructed DoFObject
       * Expected Output:  Object should be constructed
     **/

    using kernel::DoFObject;
    DoFObject dof_obj;
    ASSERT_EQ(dof_obj.n_dofs(), 0);
}


TEST(TestDotProduct, TestInsertDof) {

    /***
       * Test Scenario:   The application attempts to insert a valid DoF into DoFObject
       * Expected Output: DoF should be inserted
     **/

    using kernel::DoFObject;
    using kernel::DoF;
    DoFObject dof_obj;

    // make sure we don't have dofs
    ASSERT_EQ(dof_obj.n_dofs(), 0);

    DoF dof{"dummy_var", 10, true};

    dof_obj.insert_dof(std::move(dof));

    ASSERT_EQ(dof_obj.n_dofs(), 1);
     ASSERT_EQ(dof_obj.has_variable("dummy_var"), true);
}




