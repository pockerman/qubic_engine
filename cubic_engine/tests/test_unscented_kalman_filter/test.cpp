#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/unscented_kalman_filter.h"
#include "kernel/dynamics/system_state.h"


#include <string>
#include <gtest/gtest.h>

namespace test_data
{    
using uint_t = cengine::uint_t;
using real_t = cengine::real_t;
using cengine::DynVec;
using cengine::DynMat;
using cengine::estimation::UnscentedKalmanFilter;
using kernel::dynamics::SysState;

class TestMotionModel
{
public:

    typedef DynVec<real_t> input_t;
    typedef DynMat<real_t> matrix_t;
    typedef SysState<2> state_t;
    state_t state;

    DynVec<real_t> evaluate(const DynVec<real_t>& v1,
                            const DynVec<real_t>& u);

    state_t& get_state(){return state;}
    const state_t& get_state()const{return state;}

    real_t get(const std::string&)const{return 0.0;}

};

DynVec<real_t>
TestMotionModel::evaluate(const DynVec<real_t>& v1,
                          const DynVec<real_t>& u){

    return DynVec<real_t>(2, 0.0);

}

class TestObservationModel
{
public:

    typedef DynVec<real_t> input_t;

};
}

/// \brief
/// Scenario: Application instantiates filter without motion model
///           and attempts to call predict
/// Output:   std::logic_error is thrown
TEST(TestUnscentedKalmanFilter, TestNoMotionModel){

    using namespace test_data;
    UnscentedKalmanFilter<TestMotionModel,
                          TestObservationModel> ukf;

    DynVec<real_t> u;
    ASSERT_THROW(ukf.predict(u), std::logic_error);
}

/// \brief
/// Scenario: Application instantiates filter without observation model
///           and attempts to call update
/// Output:   std::logic_error is thrown
TEST(TestUnscentedKalmanFilter, TestNoObservationModel){

    using namespace test_data;
    UnscentedKalmanFilter<TestMotionModel,
                          TestObservationModel> ukf;

    TestMotionModel mmodel;
    ukf.set_motion_model(mmodel);

    DynVec<real_t> u;
    ASSERT_THROW(ukf.update(u), std::logic_error);
}
