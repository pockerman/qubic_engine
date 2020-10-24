#include "kernel/base/types.h"
#include "kernel/numerics/dof_object.h"
#include "kernel/base/kernel_consts.h"

#include <vector>
#include <gtest/gtest.h>

namespace{

}

TEST(TestDoFObject, TestDefaultInitialization) {

    /***
       * Test Scenario:    The application attempts to initialize a default contructed DoFObject
       * Expected Output:  Object should be constructed
     **/

    using kernel::numerics::DoFObject;
    DoFObject dof_obj;
    ASSERT_EQ(dof_obj.n_dofs(), 0);
}


TEST(TestDoFObject, TestInsertDof) {

    /***
       * Test Scenario:   The application attempts to insert a valid DoF into DoFObject
       * Expected Output: DoF should be inserted
     **/

    using kernel::numerics::DoFObject;
    using kernel::numerics::DoF;
    DoFObject dof_obj;

    // make sure we don't have dofs
    ASSERT_EQ(dof_obj.n_dofs(), 0);

    DoF dof{"dummy_var", 10, true};

    dof_obj.insert_dof(std::move(dof));

    ASSERT_EQ(dof_obj.n_dofs(), 1);
    ASSERT_EQ(dof_obj.has_variable("dummy_var"), true);
}

TEST(TestDoFObject, TestReInsertDof) {

    /***
       * Test Scenario:   The application attempts to insert a  DoF ithat already exists into DoFObject
       * Expected Output: std::logic_error should be thrown
     **/

    using kernel::numerics::DoFObject;
    using kernel::numerics::DoF;
    DoFObject dof_obj;

    // make sure we don't have dofs
    ASSERT_EQ(dof_obj.n_dofs(), 0);

    DoF dof{"dummy_var", 10, true};

    dof_obj.insert_dof(std::move(dof));

    ASSERT_EQ(dof_obj.n_dofs(), 1);
    ASSERT_EQ(dof_obj.has_variable("dummy_var"), true);

    dof = {"dummy_var", 10, true};

    try {

        dof_obj.insert_dof(std::move(dof));
    }
    catch (std::logic_error& error) {

        const std::string expected("Dof already exists");
        ASSERT_EQ(error.what(), expected);
    }
}


TEST(TestDoFObject, TestInvalidateDof) {

    /***
       * Test Scenario:   The application attempts to invalidate the dof for an existing variable
       * Expected Output: The dof should be invalidated
     **/

    using kernel::numerics::DoFObject;
    using kernel::numerics::DoF;
    DoFObject dof_obj;

    const std::string name("dummy_var");

    // make sure we don't have dofs
    ASSERT_EQ(dof_obj.n_dofs(), 0);

    DoF dof{name, 10, true};

    dof_obj.insert_dof(std::move(dof));

    ASSERT_EQ(dof_obj.n_dofs(), 1);
    ASSERT_EQ(dof_obj.has_variable("dummy_var"), true);


    dof_obj.invalidate_dofs(name);
    auto dofs = dof_obj.get_dof(name);
    ASSERT_EQ(dofs.id, kernel::KernelConsts::invalid_size_type());
    ASSERT_EQ(dofs.active, true);
}




