#include "kernel/dynamics/cart_pole_dynamics.h"
#include <stdexcept>

namespace kernel{
namespace dynamics {

CartPoleDynamics::CartPoleDynamics()
    :
      MotionModelBase<SysState<4>, DynamicsMatrixDescriptor, real_t>(),
      input_()
{}


CartPoleDynamics::CartPoleDynamics(const CartPoleInput& input)
    :
      MotionModelBase<SysState<4>, DynamicsMatrixDescriptor, real_t>(),
      input_(input)
{}


CartPoleDynamics::CartPoleDynamics(const CartPoleInput& input, const DynVec<real_t>& init_state)
    :
      MotionModelBase<SysState<4>, DynamicsMatrixDescriptor, real_t>(),
      input_(input)
{
    if(init_state.size() != 4){
        throw std::logic_error("Vector size not equal to 4");
    }

    this->get_state().set(0, {"P", init_state[0]});
    this->get_state().set(1, {"Pdot", init_state[1]});
    this->get_state().set(2, {"Phi", init_state[2]});
    this->get_state().set(3, {"PhiDot", init_state[3]});
}


CartPoleDynamics::state_t&
CartPoleDynamics::evaluate(const input_t& input ){

    integrate(std::get<0>(input));
    return this->get_state();
}

void
CartPoleDynamics::integrate(real_t v, real_t err){

}

}
}
