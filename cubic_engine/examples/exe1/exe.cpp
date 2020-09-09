#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/kalman_filter.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/maths/statistics/multivariate_normal_distribution.h"
#include "kernel/utilities/csv_file_writer.h"

#include <iostream>
#include <random>

namespace example
{

using cengine::real_t;
using cengine::uint_t;
using cengine::DynMat;
using cengine::DynVec;
using cengine::DiagMat;
using cengine::estimation::KalmanFilter;
using kernel::dynamics::MotionModelDynamicsBase;
using kernel::dynamics::DynamicsMatrixDescriptor;
using kernel::dynamics::SysState;
using kernel::maths::stats::MultiNormalDist;
using kernel::CSVWriter;

const uint_t N_ITRS = 1000;
const real_t DT = 0.5;
const real_t U0 = -2.0;

class MotionModel: public MotionModelDynamicsBase<SysState<2>,
                                          DynamicsMatrixDescriptor, DynVec<real_t>>
{

public:

    typedef MotionModelDynamicsBase<SysState<2>,
                            DynamicsMatrixDescriptor, DynVec<real_t>>::input_t input_t;

    typedef MotionModelDynamicsBase<SysState<2>,
                            DynamicsMatrixDescriptor, DynVec<real_t>>::state_t state_t;


    /// Updates and returns the value of the state given the input
    virtual state_t& evaluate(const input_t& /*input*/)override final
    {throw std::logic_error("Not Implemented");}

private:
};


class ObservationModel
{

public:

    typedef real_t input_t;

    /// constructor
    ObservationModel();

    /// Observe
    real_t evaluate(const DynVec<real_t>& input);

    /// brief Return the matrix
    const DynMat<real_t>& get_matrix(const std::string& /*name*/)const{
        return H_;
    }

private:

    /// the white noise for the observation
    std::normal_distribution<real_t> distribution_;

    /// The matrix H
    DynMat<real_t> H_;

};

ObservationModel::ObservationModel()
    :
    distribution_ (0.0,0.05),
    H_(1,2,0.0)
{
    H_(0,0) = 1.0;
    H_(0,1) = 0.0;
}

real_t
ObservationModel::evaluate(const DynVec<real_t>& input){

    std::random_device rd;
    std::mt19937 gen(rd());
    auto error = distribution_(gen);
    return 2.2; //(H_ * input)[0] + error;

}


}

int main() {

    using namespace example;

    std::cout<<"Running Example"<<std::endl;

    MotionModel motion_model;
    DynMat<real_t> F(2, 2, 0.0);
    F(0,0) = 1.0;
    F(0, 1) = DT;
    F(1, 0) = 0.0;
    F(1, 1) = 1.0;

    motion_model.set_matrix("F", F);

    /// the object that handles the error distribution
    MultiNormalDist dist_error(DynVec<real_t>(2, 0.0), DiagMat<real_t>(2,0.1) );

    DynVec<real_t> mu_init(2, 0.0);
    mu_init[1] = 5.0;

    DiagMat<real_t> sigma_init(2,0.01);
    sigma_init(1,1)=1;

    /// the initial data
    MultiNormalDist init_data(mu_init, sigma_init );

    /// the initial state
    auto state_0 = init_data.sample();

    motion_model.set_state_name_value(0, "P", state_0[0]);
    motion_model.set_state_name_value(1, "V", state_0[1]);

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

    DynMat<real_t> R(1, 1, 0.05);
    kf.set_matrix("R", R);

    CSVWriter writer("state.csv", ',', true);
    std::vector<std::string> names{"Time", "P", "V"};
    writer.write_column_names(names);
    real_t time = 0.0;

    typedef KalmanFilter<MotionModel, ObservationModel>::input_t input_t;

    for(uint_t itr=0; itr<N_ITRS; ++itr){

        std::cout<<"At time: "<<time<<std::endl;
        auto& state = kf.get_state();

        std::cout<<"State: "<<state.as_string()<<std::endl;

        auto obs = obs_model.evaluate(state.as_vector());
        DynVec<real_t> error = dist_error.sample();

        input_t in;
        in["u"] = DynVec<real_t>(1,U0);
        in["w"] = DynVec<real_t>(1,U0);

        kf.estimate(in);

        writer.write_row(std::make_tuple(time, state.get("P"), state.get("V")));

        time += DT;
    }
    
    return 0;
}

