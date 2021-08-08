#include "kernel/base/types.h"
#include "kernel/numerics/optimization/optimizer_factory.h"
#include "kernel/numerics/optimization/optimizer_type.h"
#include "kernel/numerics/optimization/optimizer_base.h"

#include <vector>
#include <map>
#include <string>
#include <any>

#include <stdexcept>
#include <gtest/gtest.h>

namespace {
using kernel::DynMat;
using kernel::DynVec;
using kernel::real_t;
using kernel::numerics::opt::OptimizerFactory;
using kernel::numerics::opt::OptimizerType;
}


TEST(TestOptimizerFactory, Invalid_Type) {

    try{

        auto factory = OptimizerFactory();

        EXPECT_DEATH(factory.build<DynMat<real_t>, DynVec<real_t>>(OptimizerType::INVALID_TYPE, std::map<std::string, std::any>()),
                     "Unrecognized OptimizerType");
    }
    catch(...){

        FAIL()<<"Could not build LinearRegressor";
    }
}

TEST(TestOptimizerFactory, GD) {

    try{

        auto factory = OptimizerFactory();
        auto ptr = factory.build<DynMat<real_t>, DynVec<real_t>>(OptimizerType::GD, std::map<std::string, std::any>());

        ASSERT_TRUE(ptr != nullptr);
        ASSERT_TRUE(ptr->type() == OptimizerType::GD);
    }
    catch(...){

        FAIL()<<"Could not build LinearRegressor";
    }
}

TEST(TestOptimizerFactory, SGD) {

    try{

        auto factory = OptimizerFactory();
        auto ptr = factory.build<DynMat<real_t>, DynVec<real_t>>(OptimizerType::SGD, std::map<std::string, std::any>());

        ASSERT_TRUE(ptr != nullptr);
        ASSERT_TRUE(ptr->type() == OptimizerType::SGD);
    }
    catch(...){

        FAIL()<<"Could not build LinearRegressor";
    }
}














