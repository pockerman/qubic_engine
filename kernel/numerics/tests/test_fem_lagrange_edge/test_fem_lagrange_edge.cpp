#include "kernel/base/config.h"
#include <gtest/gtest.h>

#ifdef USE_FEM

#include "kernel/base/types.h"
#include "kernel/numerics/fem/fe_lagrange_edge.h"
#include "kernel/numerics/fem/fe_enum_types.h"

#include <string>


TEST(TestFELagrangeEdge, TestInvalid_FE_Type) {

    using kernel::numerics::fem::FELagrangeEdge;
    using kernel::numerics::fem::FE_Type;

    try{
        FELagrangeEdge<1> fe_edge(FE_Type::INVALID_FE_TYPE);
    }
    catch(std::logic_error const & err) {
        EXPECT_EQ(err.what(), std::string("FE_Type should be in [FE_E2, FE_E3, FE_E4]"));
    }
    catch(...) {
       FAIL() << "Expected std::out_of_range";
    }

}


TEST(TestDotProduct, TestInsertDof) {


}

TEST(TestDotProduct, TestReInsertDof) {


}
#else

TEST(TestDummy, TestNothing) {


}

#endif




