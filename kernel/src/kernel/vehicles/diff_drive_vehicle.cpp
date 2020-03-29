#include "kernel/vehicles/difd_drive_vehicle.h"
#include <cmath>
namespace kernel
{

DiffDriveVehicle::DiffDriveVehicle(const DiffDriveProperties& properties)
    :
      properties_(properties),
      dynamics_()
{}

void
DiffDriveVehicle::integrate(real_t vr, real_t vl, const std::array<real_t, 2>& errors){

    /// update right/left wheel velocities
    if(std::fabs(vr) > properties_.Vm){
        vr_ = properties_.Vm;
    }
    else{
       vr_ = vr;
    }

    if(std::fabs(vl) > properties_.Vm){
        vl_ = properties_.Vm;
    }
    else{
       vl_ = vl;
    }

    dynamics_.integrate(vr_, vl_, errors);
}


const DiffDriveVehicle::state_t&
DiffDriveVehicle::get_state()const{
    return this->get_state();
}

}
