#ifndef DIFD_DRIVE_VEHICLE_H
#define DIFD_DRIVE_VEHICLE_H

#include "kernel/base/types.h"
#include "kernel/dynamics/diff_drive_dynamics.h"

namespace kernel{

struct DiffDriveProperties
{
    /// \brief The radius of the wheels
    real_t R;

    /// \brief The maximum wheel velocity
    real_t Vm;

    /// \brief Half the length of the axle connecting the
    /// two motorized wheels
    real_t L;
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
    /// by passing the right and left wheel velocities
    void integrate(real_t vr, real_t vl){integrate(vr, vl, {{0.0, 0.0}});}

    /// \brief integrate the diff drive system
    /// account for errors also
    void integrate(real_t vr, real_t vl, const std::array<real_t, 2>& errors);

    /// \brief Read the x-coordinate
    real_t get_x_position()const{return dynamics_.get_x_position(); }

    /// \brief Read the y-coordinate
    real_t get_y_position()const{return dynamics_.get_y_position();}

    /// \brief Write the position to the given
    /// type. Type must support operator[]
    template<typename Type>
    void get_position(Type& pos)const;

    /// \brief Read the orientation
    real_t get_orientation()const{return dynamics_.get_orientation();}

    /// \brief Read current velocity of the vehicle
    real_t get_velcoty()const{return 0.5*(vr_ + vl_);}

    /// \brief Read the current angular velocity
    real_t get_w_velocity()const{return (vl_ - vr_)/(2.0*properties_.L);}

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
