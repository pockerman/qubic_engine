#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/maths/constants.h"
#include "kernel/base/kernel_consts.h"
#include <cmath>
#include <iostream>
namespace kernel{
namespace dynamics{

DiffDriveDynamics::DiffDriveDynamics()
    :
  MotionModelBase<SysState<3>, DynamicsMatrixDescriptor, real_t, real_t, std::array<real_t, 2>>(),
  dt_(0.0),
  tol_(KernelConsts::tolerance())
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
DiffDriveDynamics::integrate(real_t v, real_t w, const std::array<real_t, 2>& errors){

    auto values = state_.get_values();

    if(std::fabs(w) < tol_){
        // assume zero angular velocity

       auto distance = 0.5*v*dt_;
       auto xincrement = (distance + errors[0])*std::cos(values[2]  + errors[1]);
       auto yincrement = (distance + errors[0])*std::sin(values[2]  + errors[1]);

       this->state_[0] += xincrement;
       this->state_[1] += yincrement;
    }
    else{

        this->state_[2] += w*dt_ + errors[1];
        this->state_[0] += ((v/(2.0*w)) + errors[0])*(std::sin(this->state_[2]) -
                std::sin(values[2]) + errors[1]);
        this->state_[1] -= ((v/(2.0*w)) + errors[0])*(std::cos(this->state_[2]) -
                std::cos(values[2]) + errors[1]);
    }
}

DiffDriveDynamics::state_t&
DiffDriveDynamics::evaluate(const DiffDriveDynamics::input_t& input ){
    auto [v, w, errors] = input;
    integrate(v, w, errors);

    if(this->allows_matrix_updates()){
      update_matrices(input);
    }

    return this->state_;
}

void 
DiffDriveDynamics::initialize_matrices(const DiffDriveDynamics::input_t& input){

  auto [v, w, errors] = input;

  matrix_t F(3,3,0.0);
  this->set_matrix("F", F);

  matrix_t L(2, 3, 0.0);
  this->set_matrix("L", L);

  if(std::fabs(w) < tol_){

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
  else{


  }


}

void
DiffDriveDynamics::update_matrices(const DiffDriveDynamics::input_t& input){

   auto [v, w, errors] = input;
   typedef DiffDriveDynamics::matrix_t matrix_t;

   

  
   if(std::fabs(w) < tol_){

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
   else{


   }

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
