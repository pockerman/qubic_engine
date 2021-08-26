#include "kernel/base/types.h"
#include "kernel/numerics/optimization/optimizer_factory.h"
#include "kernel/numerics/optimization/optimizer_type.h"
#include "kernel/numerics/optimization/optimizer_base.h"

#include <vector>
#include <map>
#include <string>
#include <any>
#include <tuple>

#include <stdexcept>
#include <gtest/gtest.h>

namespace {
using kernel::DynMat;
using kernel::DynVec;
using kernel::real_t;
using kernel::uint_t;
using kernel::numerics::opt::OptimizerFactory;
using kernel::numerics::opt::OptimizerType;


struct TestDataSet
{
    ///
    /// \brief storage_engine_t
    ///
    typedef DynMat<real_t> storage_engine_t;

    ///
    /// \brief storage_engine_t
    ///
    typedef DynMat<real_t> features_t;

    ///
    /// \brief labels_t
    ///
    typedef DynVec<real_t> labels_t;

    ///
    /// \brief label_value_t
    ///
    typedef real_t label_value_t;

    ///
    /// \brief row_t
    ///
    typedef DynVec<real_t> row_t;

    ///
    /// \brief operator []
    /// \param i
    /// \return
    ///
    std::tuple<DynVec<real_t>, real_t> operator[](uint_t)const{return std::make_tuple(DynVec<real_t>(), 1.0);}

    uint_t n_examples()const{return 1;}
};

struct TestFunction
{

    template<typename RowTp, typename T>
    real_t error_at(const RowTp&, const T&)const{return 0.0;}

    template<typename RowTp, typename T>
    DynVec<real_t> param_gradient_at(const RowTp&, const T& )const{return DynVec<real_t>();}

    DynVec<real_t> parameters()const{return DynVec<real_t>();}

    uint_t n_parameters()const{return 1;}

    template<typename Data>
    real_t evaluate(const Data&){return 0.0;}

    template<typename Data>
    DynVec<real_t> params_gradients(const Data&)const{return DynVec<real_t>();}

    template<typename Container>
    void update_parameters(const Container&){}

};

}


TEST(TestOptimizerFactory, Invalid_Type) {

    try{

        auto factory = OptimizerFactory();
        factory.build<TestDataSet, TestFunction, std::map<std::string, std::any>>(OptimizerType::INVALID_TYPE, std::map<std::string, std::any>());
    }
    catch(...){

        FAIL()<<"Could not build Optimizer";
    }
}

TEST(TestOptimizerFactory, GD) {

    try{

        auto factory = OptimizerFactory();
        auto ptr = factory.build<TestDataSet, TestFunction, std::map<std::string, std::any>>(OptimizerType::GD, std::map<std::string, std::any>());

        ASSERT_TRUE(ptr != nullptr);
        ASSERT_TRUE(ptr->type() == OptimizerType::GD);
    }
    catch(...){

        FAIL()<<"Could not build GD Optimizer";
    }
}

TEST(TestOptimizerFactory, SGD) {

    try{

        auto factory = OptimizerFactory();
        auto ptr = factory.build<TestDataSet, TestFunction, std::map<std::string, std::any>>(OptimizerType::SGD, std::map<std::string, std::any>());

        ASSERT_TRUE(ptr != nullptr);
        ASSERT_TRUE(ptr->type() == OptimizerType::SGD);
    }
    catch(...){

        FAIL()<<"Could not build SGD Optimizer";
    }
}














