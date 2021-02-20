#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/maths/constants.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/utilities/input_resolver.h"

#include <cmath>
#include <iostream>
namespace kernel{
namespace dynamics{


namespace {

void integrate_state_v1(SysState<3>& state, real_t tol, real_t dt, real_t v, real_t w, const std::array<real_t, 2>& errors){

    auto values = state.get_values();

    if(std::fabs(w) < tol){

        /// assume zero angular velocity
       auto distance = 0.5*v*dt;
       auto xincrement = (distance + errors[0])*std::cos(values[2]  + errors[1]);
       auto yincrement = (distance + errors[0])*std::sin(values[2]  + errors[1]);

       state[0] += xincrement;
       state[1] += yincrement;
    }
    else{

        state[2] += w*dt + errors[1];

        /// clip the value
        if(std::fabs(state[2]) > MathConsts::PI){
            state[2] = utils::sign(state[2])*MathConsts::PI;
        }

        state[0] += ((v/(2.0*w)) + errors[0])*(std::sin(state[2]) - std::sin(values[2]));
        state[1] -= ((v/(2.0*w)) + errors[0])*(std::cos(state[2]) - std::cos(values[2]));
    }

}


void integrate_state_v2(SysState<3>& state, real_t dt, real_t v, real_t w, const std::array<real_t, 2>& errors){

    auto values = state.get_values();
    auto distance = 0.5*v*dt;
    auto xincrement = (distance + errors[0])*std::cos(values[2]  + errors[1]);
    auto yincrement = (distance + errors[0])*std::sin(values[2]  + errors[1]);

    state[0] += xincrement;
    state[1] += yincrement;
    state[2] += dt*w;

}


void integrate_state_v3(SysState<3>& state, real_t r, real_t l, real_t dt, real_t w1,
                        real_t w2, const std::array<real_t, 2>& errors){

    auto values = state.get_values();
    auto xincrement = dt*0.5*r*(w1 + w2 + errors[0])*std::cos(values[2]  + errors[1]);
    auto yincrement = dt*0.5*r*(w1 + w2 + errors[0])*std::sin(values[2]  + errors[1]);

    state[0] += xincrement;
    state[1] += yincrement;
    state[2] += dt*r*(w1 - w2)/(2.0*l);
}
}

DiffDriveDynamics::DiffDriveDynamics(DiffDriveDynamics::DynamicVersion type)
    :
  MotionModelDynamicsBase<SysState<3>, DynamicsMatrixDescriptor, std::map<std::string, boost::any>>(),
  v_(0.0),
  w_(0.0),
  type_(type)
{
    this->state_.set(0, {"X", 0.0});
    this->state_.set(1, {"Y", 0.0});
    this->state_.set(2, {"Theta", 0.0});
}

DiffDriveDynamics::DiffDriveDynamics(DiffDriveDynamics::state_t&& state)
    :
      MotionModelDynamicsBase<SysState<3>, DynamicsMatrixDescriptor,
                              std::map<std::string, boost::any>>(),
      v_(0.0),
      w_(0.0)
{
    this->state_ = state;
}

void
DiffDriveDynamics::integrate(const DiffDriveDynamics::input_t& input){

    auto values = state_.get_values();



    // before we do the integration
    // update the matrices
    if(this->allows_matrix_updates()){
      update_matrices(input);
    }


    if(type_ == DiffDriveDynamics::DynamicVersion::V1){

        auto w = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("w", input);
        auto v = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("v", input);
        auto errors = utils::InputResolver<std::map<std::string, boost::any>, std::array<real_t, 2>>::resolve("errors", input);
        integrate_state_v1(this->state_, tol_, get_time_step(), v, w, errors);

        // update the velocities and angular
        // velocities
        v_ = v;
        w_ = w;

    }
    else if(type_ == DiffDriveDynamics::DynamicVersion::V2){

        auto w = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("w", input);
        auto v = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("v", input);
        auto errors = utils::InputResolver<std::map<std::string, boost::any>, std::array<real_t, 2>>::resolve("errors", input);
        integrate_state_v2(this->state_, get_time_step(), v, w, errors);

        // update the velocities and angular
        // velocities
        v_ = v;
        w_ = w;
    }
    else if(type_ == DiffDriveDynamics::DynamicVersion::V3){

        // in this scenario we have the wheels speed as input

        auto w1 = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("w1", input);
        auto w2 = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("w2", input);
        auto errors = utils::InputResolver<std::map<std::string, boost::any>, std::array<real_t, 2>>::resolve("errors", input);
        auto r = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("r", input);
        auto l = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("l", input);
        integrate_state_v3(this->state_, r, l, get_time_step(), w1, w2, errors);

        v_ = 0.5*r*(w1 + w2);
        w_ = r*(w1 - w2)/(2.0*l);
    }
}

DiffDriveDynamics::state_t&
DiffDriveDynamics::evaluate(const DiffDriveDynamics::input_t& input ){
    integrate(input);
    return this->state_;
}

void 
DiffDriveDynamics::initialize_matrices(const DiffDriveDynamics::input_t& input){


  // if we initialize the matrices
  // then we should set the matrix update flag to true
  set_matrix_update_flag(true);

  if(!this->has_matrix("F")){
    matrix_t F(3,3, 0.0);
    this->set_matrix("F", F);
  }

  if(! this->has_matrix("L")){
    matrix_t L(3, 2, 0.0);
    this->set_matrix("L", L);
  }

  update_matrices(input);

}

void
DiffDriveDynamics::update_matrices(const DiffDriveDynamics::input_t& input){

   auto w = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("w", input);
   auto v = utils::InputResolver<std::map<std::string, boost::any>, real_t>::resolve("v", input);
   auto errors = utils::InputResolver<std::map<std::string, boost::any>, std::array<real_t, 2>>::resolve("errors", input);

   auto distance = 0.5*v*get_time_step();
   auto orientation = w*get_time_step();
   auto values = this->state_.get_values();
  
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
      L(1, 1) = -(distance + errors[0])*std::cos(values[2] + orientation + errors[1]);

      L(2, 0) = 0.0;
      L(2, 1) = 1.0;

   }
   else{

      auto& F = this->get_matrix("F");

      F(0, 0) = 1.0;
      F(0, 1) = 0.0;
      F(0, 2) = -(distance + errors[0])*std::cos(values[2] + orientation + errors[1]) +
                 (distance + errors[0])*std::cos(values[2]);

      F(1, 0) = 0.0;
      F(1, 1) = 1.0;
      F(1, 2) = -(distance + errors[0])*std::sin(values[2] + orientation + errors[1]) +
                 (distance + errors[0])*std::sin(values[2]);

      F(2, 0) = 0.0;
      F(2, 1) = 0.0;
      F(2, 2) = 1.0;

      auto& L = this->get_matrix("L");

      L(0, 0) = std::sin(values[2] + orientation + errors[1])- std::sin(values[2]);
                
      L(0, 1) = -((v/2.0*w) + errors[0])*std::cos(values[2] + orientation + errors[1])*
                std::sin(values[2] + orientation + errors[1]);

      L(1, 0) = -std::cos(values[2] + orientation + errors[1]) + std::cos(values[2]);
      L(1, 1) = ((v/2.0*w) + errors[0])*std::sin(values[2] + orientation + errors[1]);

      L(2, 0) = 0.0;
      L(2, 1) = 1.0; 
   }

}

}
}
