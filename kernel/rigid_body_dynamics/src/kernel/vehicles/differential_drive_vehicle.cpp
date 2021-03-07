#include "kernel/vehicles/differential_drive_vehicle.h"
#include <cmath>

namespace kernel{
namespace vehicles{


DiffDriveVehicle::DiffDriveVehicle(const DiffDriveConfig& properties)
    :
      VehicleBase<dynamics::DiffDriveDynamics>(properties.dynamics_version),
      properties_(properties),
      vr_(0.0),
      vl_(0.0),
      w_(0.0)
{
    this->dynamics_.set_matrix_update_flag(properties_.update_description_matrices_on_evaluate);
}

real_t
DiffDriveVehicle::get_clipped_velocity(real_t v)const{

    // clip the velocity
    if( v < properties_.vmin){
        return properties_.vmin;
    }

    if( v > properties_.vmax){
        return properties_.vmax;
    }

    return v;
}

real_t
DiffDriveVehicle::get_clipped_angular_velocity(real_t w)const{

    // clip the velocity
    if( w < properties_.wmin){
        return properties_.wmin;
    }

    if( w > properties_.wmax){
        return properties_.wmax;
    }

    return w;
}

void
DiffDriveVehicle::set_velocity(real_t v){

    auto clipped_v = get_clipped_velocity(v);

    vr_ = (clipped_v + properties_.width*w_)/properties_.wheel_radius;
    vl_ = (clipped_v - properties_.width*w_)/properties_.wheel_radius;
}

void
DiffDriveVehicle::set_velocities(real_t v, real_t w){

    // set the yaw rate first
    w_ = get_clipped_angular_velocity(w);

    // set the linear velocity
    set_velocity(v);
}

void
DiffDriveVehicle::integrate(real_t v, real_t w, const std::array<real_t, 2>& errors){

    // set the right and left wheel velocities
    set_velocities(v, w);

    auto velocity = get_velocity();

    dynamics::DiffDriveDynamics::input_t input;
    input.insert_or_assign("v", velocity);
    input.insert_or_assign("w", w_);
    input.insert_or_assign("errors", errors);

    this->dynamics_.integrate(input);
}


}
}
