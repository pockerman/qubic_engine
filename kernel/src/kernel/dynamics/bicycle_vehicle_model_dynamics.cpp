#include "kernel/dynamics/bicycle_vehicle_model_dynamics.h"

namespace kernel{
namespace dynamics{


BicycleVehicleModelDynamics::BicycleVehicleModelDynamics()
    :
      BicycleVehicleModelDynamics::base_t()
{
    this->state_.set(0, {"X", 0.0});
    this->state_.set(1, {"Y", 0.0});
    this->state_.set(2, {"Phi", 0.0});
    this->state_.set(3, {"V_x", 0.0});
    this->state_.set(4, {"V_y", 0.0});
    this->state_.set(5, {"r", 0.0});
    this->state_.set(6, {"s", 0.0});
    this->state_.set(7, {"d", 0.0});
    this->state_.set(8, {"delta", 0.0});
    this->state_.set(9, {"V_s", 0.0});
}

void
BicycleVehicleModelDynamics::integrate(const BicycleVehicleModelDynamics::input_t& input ){

    /*auto values = state_.get_values();



    /// before we do the integration
    /// update the matrices
    if(this->allows_matrix_updates()){

      DiffDriveDynamics::input_t input(v,w,errors);
      update_matrices(input);
    }

    if(std::fabs(w) < tol_){
        /// assume zero angular velocity
       auto distance = 0.5*v*get_time_step();
       auto xincrement = (distance + errors[0])*std::cos(values[2]  + errors[1]);
       auto yincrement = (distance + errors[0])*std::sin(values[2]  + errors[1]);

       this->state_[0] += xincrement;
       this->state_[1] += yincrement;
    }
    else{

        this->state_[2] += w*get_time_step() + errors[1];

        /// clip the value
        if(std::fabs(this->state_[2]) > MathConsts::PI){
            this->state_[2] = utils::sign(this->state_[2])*MathConsts::PI;
        }


        this->state_[0] += ((v/(2.0*w)) + errors[0])*(std::sin(this->state_[2]) -
                std::sin(values[2]));

        this->state_[1] -= ((v/(2.0*w)) + errors[0])*(std::cos(this->state_[2]) -
                std::cos(values[2]));
    }*/
}

BicycleVehicleModelDynamics::state_t&
BicycleVehicleModelDynamics::evaluate(const BicycleVehicleModelDynamics::input_t& input ){
    //auto [v, w, errors] = input;
    //integrate(v, w, errors);
    //return this->state_;
}

void
BicycleVehicleModelDynamics::initialize_matrices(const BicycleVehicleModelDynamics::input_t& input){


  /// if we initialize the matrices
  /// then we should set the matrix update flag to true
  /*set_matrix_update_flag(true);

  if(!this->has_matrix("F")){
    matrix_t F(3,3, 0.0);
    this->set_matrix("F", F);
  }

  if(! this->has_matrix("L")){
    matrix_t L(3, 2, 0.0);
    this->set_matrix("L", L);
  }

  update_matrices(input);*/

}

void
BicycleVehicleModelDynamics::update_matrices(const BicycleVehicleModelDynamics::input_t& input){

   /*auto [v, w, errors] = input;

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
   }*/

}

}
}
