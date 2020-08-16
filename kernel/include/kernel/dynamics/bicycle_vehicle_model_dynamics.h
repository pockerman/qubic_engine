#ifndef BICYCLE_VEHICLE_MODEL_DYNAMICS_H
#define BICYCLE_VEHICLE_MODEL_DYNAMICS_H

#include "kernel/base/types.h"
#include "kernel/dynamics/system_state.h"
#include "kernel/dynamics/motion_model_base.h"
#include "kernel/dynamics/dynamics_matrix_descriptor.h"

namespace kernel{
namespace dynamics{

///
/// \brief The BicycleVehicleModelDynamics class.
/// Implements the bicycle vehicle model dynamics
///
class BicycleVehicleModelDynamics: public MotionModelDynamicsBase<SysState<6>, DynamicsMatrixDescriptor,
                                                                  real_t, real_t, std::array<real_t, 2>>
{

};

}
}

#endif // BICYCLE_VEHICLE_MODEL_DYNAMICS_H
