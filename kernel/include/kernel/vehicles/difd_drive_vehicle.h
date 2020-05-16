#ifndef DIFD_DRIVE_VEHICLE_H
#define DIFD_DRIVE_VEHICLE_H

#include "kernel/base/types.h"
#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/base/kernel_consts.h"

namespace kernel{

struct DiffDriveProperties
{
    /// \brief The radius of the wheels
    real_t R;

    /// \brief The maximum wheel velocity
    real_t Vmax;

    /// \brief The maximum vehicle acceleration
    real_t Amax;

    /// \brief Half the length of the axle connecting the
    /// two motorized wheels
    real_t L;

    /// \brief Tolerance used
    real_t tol{KernelConsts::tolerance()};

    /// \brief The CoM
    GeomPoint<3> com;
    
    /// \brief flag indicating if the matrix description 
    /// of the vehicle dynamics will be used
    bool update_description_matrices_on_evaluate{false};
};

/// \brief Base class that models a differential drive
class DiffDriveVehicle
{

public:

    typedef dynamics::DiffDriveDynamics dynamics_t;
    typedef dynamics::DiffDriveDynamics::state_t state_t;

    /// \brief constructor
    DiffDriveVehicle(const DiffDriveProperties& properties);

    /// \brief integrate the diff drive system
    /// by passing the linear and angular velocities and an velocities
    void integrate(real_t v, real_t w){integrate(v, w, {{0.0, 0.0}});}

    /// \brief integrate the diff drive system
    /// account for errors also
    void integrate(real_t v, real_t w, const std::array<real_t, 2>& errors);

    /// \brief Read the x-coordinate
    real_t get_x_position()const{return dynamics_.get_x_position(); }

    /// \brief Set the x-coordinate
    real_t set_x_position(real_t x){dynamics_.set_x_position(x);}

    /// \brief Read the y-coordinate
    real_t get_y_position()const{return dynamics_.get_y_position();}

    /// \brief Set the y-coordinate
    real_t set_y_position(real_t y){dynamics_.set_y_position(y);}

    /// \brief Write the position to the given
    /// type. Type must support operator[]
    template<typename Type>
    void get_position(Type& pos)const;

    /// \brief Read the orientation
    real_t get_orientation()const{return dynamics_.get_orientation();}

    /// \brief Set the orientation
    void set_orientation(real_t theta){dynamics_.set_orientation(theta);}

    /// \brief Set the linear velocity of the vehicle.
    /// This sets the vr, vl assuming that the angular
    /// velocity of the vehicle is zero. If not use
    /// the set velocities function
    void set_velocity(real_t v);

    /// \brief Set the linear and angular velocities
    /// of the vehicle
    void set_velocities(real_t v, real_t w);

    /// \brief Set time step
    real_t set_time_step(real_t dt){dynamics_.set_time_step(dt);}

    /// \brief Read current velocity of the vehicle
    real_t get_velcoty()const{return 0.5*properties_.R*(vr_ + vl_);}

    /// \brief Read the current angular velocity
    real_t get_w_velocity()const{return (properties_.R*(vl_ - vr_))/(2.0*properties_.L);}

    /// \brief Returns the state of the vehicle
    const state_t& get_state()const;

private:

    /// \brief The properties of the robot
    DiffDriveProperties properties_;

    /// \brief The object that handles the dynamics
    dynamics::DiffDriveDynamics dynamics_;

    /// \brief The right wheel velocity
    real_t vr_;

    /// \brief The left wheel velocity
    real_t vl_;
};


template<typename Type>
void
DiffDriveVehicle::get_position(Type& pos)const{

    pos[0] = get_x_position();
    pos[1] = get_y_position();
}

}

#endif // DIFD_DRIVE_VEHICLE_H
