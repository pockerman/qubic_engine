#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/maths/constants.h"
#include <cmath>

namespace kernel{
namespace dynamics{

DiffDriveDynamics::DiffDriveDynamics()
    :
  MotionModelBase<SysState<3>, DynamicsMatrixDescriptor, real_t, real_t, std::array<real_t, 2>>()
{
    this->state_.set(0, {"X", 0.0});
    this->state_.set(1, {"Y", 0.0});
    this->state_.set(2, {"Theta", 0.0});
}

DiffDriveDynamics::DiffDriveDynamics(DiffDriveDynamics::state_t&& state)
    :
      MotionModelBase<SysState<3>, DynamicsMatrixDescriptor, real_t, real_t, std::array<real_t, 2>>()
{
    this->state_ = state;
}

void
DiffDriveDynamics::integrate(real_t distance, real_t orientation, const std::array<real_t, 2>& errors){

    auto values = state_.get_values();

    this->state_[0] += values[0] + (distance + errors[0])*std::cos(values[2] + orientation + errors[1]);
    this->state_[1] += values[1] + (distance + errors[0])*std::sin(values[2] + orientation + errors[1]);
    this->state_[2] += values[2] + orientation + errors[1];

    if( this->state_[2] <  - MathConsts::PI){
        this->state_[2] = - MathConsts::PI;
    }
    else if(this->state_[2] > MathConsts::PI){
        this->state_[2] =  MathConsts::PI;
    }
}

DiffDriveDynamics::state_t&
DiffDriveDynamics::evaluate(const DiffDriveDynamics::input_t& input ){
    auto [distance, orientation, errors] = input;
    integrate(distance, orientation, errors);

    if(this->allows_matrix_updates()){
      update_matrices(input);
    }

    return this->state_;
}

void
DiffDriveDynamics::update_matrices(const DiffDriveDynamics::input_t& input){

   auto [distance, orientation, errors] = input;
   auto values = this->state_.get_values();
   auto& F = this->get_matrix("F");
   F(0, 0) = 1.0;
   F(0, 1) = 0.0;
   F(0, 2) = (distance + errors[0])*std::sin(values[2] + orientation + errors[1]);

   F(1, 0) = 0.0;
   F(1, 1) = 1.0;
   F(1, 2) = -(distance + errors[0])*std::cos(values[2] + orientation + errors[1]);

   F(2, 0) = 0.0;
   F(2, 1) = 0.0;
   F(2, 2) = 1.0;

   auto& L = this->get_matrix("L");

   L(0, 0) = std::cos(values[2] + orientation + errors[1]);
   L(0, 1) = (distance + errors[0])*std::sin(values[2] + orientation + errors[1]);

   L(1, 0) = std::sin(values[2] + orientation + errors[1]);
   L(1, 1) = -(distance + errors[0])*std::sin(values[2] + orientation + errors[1]);

   L(2, 0) = 0.0;
   L(2, 1) = 1.0;

}

}
}
