#include "kernel/vehicles/difd_drive_vehicle.h"

namespace kernel
{

DiffDriveVehicle::DiffDriveVehicle(real_t r)
    :
      wheel_radius_(r),
      dynamics_(),
      velocity_(0.0),
      w_(0.0)
{}

void
DiffDriveVehicle::integrate(real_t velocity, real_t orientation, const std::array<real_t, 2>& errors){

    velocity_ = velocity;
    dynamics_.integrate(velocity, orientation, errors);
}


const DiffDriveVehicle::state_t&
DiffDriveVehicle::get_state()const{
    return this->get_state();
}

}
