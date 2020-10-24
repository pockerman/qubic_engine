#include "kernel/vehicles/difd_drive_vehicle.h"
#include <cmath>
namespace kernel
{

DiffDriveVehicle::DiffDriveVehicle(const DiffDriveProperties& properties)
    :
      properties_(properties),
      dynamics_()
{
		dynamics_.set_matrix_update_flag(properties_.update_description_matrices_on_evaluate);
}

void
DiffDriveVehicle::set_velocity(real_t v){

    vr_ = v/properties_.R;

    if(std::fabs(vr_) > properties_.Vmax){
        vr_ = properties_.Vmax;
    }

    vl_ = v/properties_.R;

    if(std::fabs(vl_) > properties_.Vmax){
        vl_ = properties_.Vmax;
    }
}

void
DiffDriveVehicle::set_velocities(real_t v, real_t w){

    if (std::fabs(w) < properties_.tol){
        /// assume that angular velocity is zerp
        set_velocity(v);
        return;
    }

    /// TODO: Verify that this is correct
    vl_ = v/properties_.R;
    vr_ = (v + w*2.0*properties_.L)/properties_.R;
}

void
DiffDriveVehicle::integrate(real_t v, real_t w, const std::array<real_t, 2>& errors){

    /// set the right and left wheel velocities
    set_velocities(v, w);

    auto velocity = get_velcoty();
    auto wvelocity = get_w_velocity();

    dynamics_.integrate(v, w, errors);
}


const DiffDriveVehicle::state_t&
DiffDriveVehicle::get_state()const{
    return dynamics_.get_state();
}

}
