#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/kalman_filter.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"
#include "kernel/dynamics/system_state.h"
#include <iostream>

namespace example
{

using cengine::real_t;
using cengine::uint_t;
using cengine::DynMat;
using cengine::KalmanFilter;
using kernel::dynamics::MotionModelBase;
using kernel::dynamics::DynamicsMatrixDescriptor;
using kernel::dynamics::SysState;

const uint_t N_ITRS = 100;
const real_t DT = 0.5;

class MotionModel: public MotionModelBase<SysState<2>,
                                          DynamicsMatrixDescriptor, real_t>
{

public:

    typedef MotionModelBase<SysState<2>,
                            DynamicsMatrixDescriptor, real_t>::input_t input_t;

    /// \brief Updates and returns the value of the state given the input
    virtual state_t& evaluate(const input_t& input);

};

class ObservationModel
{

};


}

int main() {

    using namespace example;

    std::cout<<"Running Example"<<std::endl;

    MotionModel motion_model;
    ObservationModel obs_model;

    KalmanFilter<MotionModel, ObservationModel> kf(motion_model, obs_model);

    DynMat<real_t> P(2, 2, 0.0);
    P(0, 0) = 1.0;
    P(1, 1) = 1.0;

    kf.set_matrix("P", P);

    DynMat<real_t> Q(2, 2, 0.0);
    Q(0, 0) = 0.1;
    Q(1, 1) = 0.1;

    kf.set_matrix("Q", Q);

    DynMat<real_t> B(2, 1, 0.0);
    B(0, 0) = 0.0;
    B(1, 0) = DT;
    kf.set_matrix("B", B);



    for(uint_t itr=0; itr<N_ITRS; ++itr){


    }
    



    return 0;
}

