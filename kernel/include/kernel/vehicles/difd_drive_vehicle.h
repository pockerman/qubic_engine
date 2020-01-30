#ifndef DIFD_DRIVE_VEHICLE_H
#define DIFD_DRIVE_VEHICLE_H

#include "kernel/base/types.h"
#include "kernel/dynamics/diff_drive_dynamics.h"

namespace kernel
{

/// \brief Base class that models a differential drive
class DiffDriveVehicle
{

public:

    typedef dynamics::DiffDriveDynamics dynamics_t;
    typedef dynamics::DiffDriveDynamics::state_t state_t;


    /// \brief constructor
    DiffDriveVehicle(real_t r);

    /// \brief integrate the diff drive system
    void integrate(real_t velocity, real_t orientation){integrate(velocity, orientation, {{0.0, 0.0}});}

    /// \brief integrate the diff drive system
    /// account for errors also
    void integrate(real_t velocity, real_t orientation, const std::array<real_t, 2>& errors);

private:

    /// \brief The radius of the wheels
    real_t wheel_radius_;

    /// \brief The object that handles the dynamics
    dynamics::DiffDriveDynamics dynamics_;


};
}

#endif // DIFD_DRIVE_VEHICLE_H
