#include "kernel/vehicles/difd_drive_vehicle.h"

namespace kernel
{

DiffDriveVehicle::DiffDriveVehicle(real_t r)
    :
      wheel_radius_(r),
      dynamics_()
{}

void
DiffDriveVehicle::integrate(real_t velocity, real_t orientation, const std::array<real_t, 2>& errors){

dynamics_.integrate(velocity, orientation, errors);
}
}
