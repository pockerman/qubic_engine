#include "kernel/dynamics/quadrotor_dynamics.h"

namespace kernel{
namespace dynamics {


QuadrotorDynamics::QuadrotorDynamics()
    :
      MotionModelDynamicsBase<SysState<12>,
                                  DynamicsMatrixDescriptor,
                                  std::map<std::string, boost::any>>()
{}


QuadrotorDynamics::state_t&
QuadrotorDynamics::evaluate(const input_t& input ){

}


void
QuadrotorDynamics::integrate(const QuadrotorDynamics::input_t& input){

}

}
}
