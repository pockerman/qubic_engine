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
using cengine::IdentityMatrix;
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

/// \brief
/// Scenario: Application instantiates filter without having initialized
///           the sigma points
/// Output:   std::logic_error is thrown
TEST(TestUnscentedKalmanFilter, TestNoSigmaPoints){

    using namespace test_data;
    UnscentedKalmanFilter<TestMotionModel,
                          TestObservationModel> ukf;

    TestMotionModel mmodel;
    ukf.set_motion_model(mmodel);

    TestObservationModel omodel;
    ukf.set_observation_model(omodel);

    DynVec<real_t> u;
    ASSERT_THROW(ukf.predict(u), std::logic_error);
}


/// \brief
/// Scenario: Application instantiates filter without and initialializes
///           the sigma points
/// Output:   correct number of weights and sigma points
TEST(TestUnscentedKalmanFilter, TestSigmaPointsInit){

    using namespace test_data;
    UnscentedKalmanFilter<TestMotionModel,
                          TestObservationModel> ukf;

    TestMotionModel mmodel;
    ukf.set_motion_model(mmodel);

    TestObservationModel omodel;
    ukf.set_observation_model(omodel);

    /// we also need to covariance matrix
    IdentityMatrix<real_t> P(mmodel.state.dimension);
    ukf.set_matrix("P", P);

    ukf.initialize_sigma_points(1.2);

    ASSERT_EQ(ukf.n_sigma_points(), 2*mmodel.state.dimension + 1);
    ASSERT_EQ(ukf.n_weights(), 2*mmodel.state.dimension + 1);
}


