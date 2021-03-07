#include "kernel/dynamics/cart_pole_dynamics.h"
#include "kernel/utilities/input_resolver.h"

#include <cmath>
#include <stdexcept>

namespace kernel{
namespace dynamics {

CartPoleDynamics::CartPoleDynamics()
    :
      MotionModelDynamicsBase<SysState<4>, DynamicsMatrixDescriptor, std::map<std::string, boost::any>>(),
      config_()
{
    this->set_time_step(config_.dt);
    this->get_state().set(0, {"P", 0.0});
    this->get_state().set(1, {"Pdot", 0.0});
    this->get_state().set(2, {"Phi", 0.0});
    this->get_state().set(3, {"PhiDot", 0.0});
}


CartPoleDynamics::CartPoleDynamics(const CartPoleConfig& config)
    :
      MotionModelDynamicsBase<SysState<4>, DynamicsMatrixDescriptor, std::map<std::string, boost::any>>(),
      config_(config)
{
    this->set_time_step(config_.dt);
    this->get_state().set(0, {"P", 0.0});
    this->get_state().set(1, {"Pdot", 0.0});
    this->get_state().set(2, {"Phi", 0.0});
    this->get_state().set(3, {"PhiDot", 0.0});
}


CartPoleDynamics::CartPoleDynamics(const CartPoleConfig& config, const DynVec<real_t>& init_state)
    :
      MotionModelDynamicsBase<SysState<4>, DynamicsMatrixDescriptor, std::map<std::string, boost::any>>(),
      config_(config)
{
    if(init_state.size() != 4){
        throw std::logic_error("Vector size not equal to 4");
    }

    this->set_time_step(config_.dt);
    this->get_state().set(0, {"P", init_state[0]});
    this->get_state().set(1, {"Pdot", init_state[1]});
    this->get_state().set(2, {"Phi", init_state[2]});
    this->get_state().set(3, {"PhiDot", init_state[3]});
}


CartPoleDynamics::state_t&
CartPoleDynamics::evaluate(const CartPoleDynamics::input_t& input ){

    integrate(input);
    return this->get_state();
}

void
CartPoleDynamics::integrate(const CartPoleDynamics::input_t& input){

    auto values = state_.get_values();

    if(this->allows_matrix_updates()){
      update_matrices(input);
    }

    auto F = utils::InputResolver<CartPoleDynamics::input_t, real_t>::resolve("F", input);
    auto m = config_.m;
    auto M = config_.M;
    auto fphi = config_.f_phi;
    auto b = config_.b;
    auto g = config_.g;
    auto l = config_.rod_length;

    auto omega = this->state_[3];
    auto v = this->state_[1];
    auto theta = this->state_[2];

    auto cos_theta = std::cos(theta);
    auto cos_theta_2 = cos_theta*cos_theta;
    auto sin_theta = std::sin(theta);

    const auto dt = this->get_time_step();
    this->state_[0] += dt*values[1];
    this->state_[1] += dt*(m*l*omega*omega*sin_theta - m * g * sin_theta * cos_theta + m * fphi * cos_theta * omega + F - b * v) / (M + m * (1. - cos_theta_2));
    this->state_[2] += dt*values[3];
    this->state_[3] += dt*((M + m) * (g * sin_theta - fphi * omega) - m * l * omega * omega * sin_theta * cos_theta - (F - b * v) * cos_theta) / (l * (M + m * (1 - cos_theta_2)));


}


void
CartPoleDynamics::update_matrices(const input_t& input){

    auto& F = this->get_matrix("F");
    auto b = config_.b;
    auto M = config_.M;
    auto m = config_.m;
    auto l = config_.rod_length;
    auto fphi = config_.f_phi;
    auto g = config_.g;


    F(0, 0) = 0.;
    F(0, 1) = 1.;
    F(0, 2) = 0.;
    F(0, 3) = 0.;

    F(1, 0) = 0.;
    F(1, 1) = -b/M;
    F(1, 2) = -(g*m)/M;
    F(1, 3) = -(fphi*m)/M;

    F(2, 0) = 0.;
    F(2, 1) = 0.;
    F(2, 2) = 0.;
    F(2, 3) = 1.;

    F(3, 0) = 0.;
    F(3, 1) = b/(M*l);
    F(3, 2) = (M*g + g*m)/(M*l);
    F(3, 3) = -(M*fphi + fphi*m)/(M*l);

}

void
CartPoleDynamics::initialize_matrices(const input_t& input){

    // if we initialize the matrices
    // then we should set the matrix update flag to true
    this->set_matrix_update_flag(true);

    if(!this->has_matrix("F")){
      matrix_t F(4, 4, 0.0);
      this->set_matrix("F", F);
    }

}

}
}
