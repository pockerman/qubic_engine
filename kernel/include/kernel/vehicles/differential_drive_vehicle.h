#ifndef DIFFERENTIAL_DRIVE_VEHICLE_H
#define DIFFERENTIAL_DRIVE_VEHICLE_H

#include "kernel/base/types.h"
#include "kernel/dynamics/diff_drive_dynamics.h"
#include "kernel/vehicles/vehicle_base.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/bounding_box_type.h"

namespace kernel{
namespace vehicles{

///
/// \brief The DiffDriveConfig struct. Sturct
/// to assemble the configuration properties
/// for thedifferential drive vehicle
///
struct DiffDriveConfig
{
    ///
    /// \brief The radius of the wheels
    ///
    real_t wheel_radius{1.0};

    ///
    /// \brief Half the length of the axle connecting the
    /// two motorized wheels
    ///
    real_t width{1.0};

    ///
    /// \brief The length of the robot
    ///
    real_t length{1.0};

    ///
    /// \brief vmax_. The maximum vehicle velocity
    ///
    real_t vmax{0.0};

    ///
    /// \brief vmin_. The minimum vehicle velocity
    ///
    real_t vmin{0.0};

    ///
    /// \brief wmax_. The maximum vehicle angular velocity
    ///
    real_t wmax{0.0};

    ///
    /// \brief wmin_. The minimum vehicle angular velocity
    ///
    real_t wmin{0.0};

    ///
    /// \brief Tolerance used
    ///
    real_t tol{KernelConsts::tolerance()};

    ///
    /// \brief The CoM
    ///
    GeomPoint<3> com;
    
    ///
    /// \brief flag indicating if the matrix description 
    /// of the vehicle dynamics will be used
    ///
    bool update_description_matrices_on_evaluate{false};

    ///
    /// \brief The version of the dynamics the vehicle is using
    ///
    dynamics::DiffDriveDynamics::DynamicVersion dynamics_version{dynamics::DiffDriveDynamics::DynamicVersion::V1};

    ///
    /// \brief bbtype. The type of the bounding box
    ///
    kernel::geom::BBType bbtype;
};

///
/// \brief Base class that models a differential drive vehicle
///
class DiffDriveVehicle: public VehicleBase<dynamics::DiffDriveDynamics>
{

public:

    ///
    /// \brief dynamics_t. The type of the object describing
    /// the dynamics
    ///
    typedef VehicleBase<dynamics::DiffDriveDynamics>::dynamics_t dynamics_t;

    ///
    /// \brief state_t. The type of the object describing the
    /// state
    ///
    typedef VehicleBase<dynamics::DiffDriveDynamics>::state_t state_t;

    ///
    /// \brief constructor
    ///
    DiffDriveVehicle(const DiffDriveConfig& properties);

    ///
    /// \brief integrate the diff drive system
    /// by passing the linear and angular velocities
    ///
    void integrate(real_t v, real_t w){integrate(v, w, {{0.0, 0.0}});}

    ///
    /// \brief integrate the diff drive system
    /// account for errors also
    ///
    void integrate(real_t v, real_t w, const std::array<real_t, 2>& errors);

    ///
    /// \brief Set the linear velocity of the vehicle.
    /// This sets the vr, vl assuming that the angular
    /// velocity of the vehicle is zero. If not use
    /// the set velocities function
    ///
    void set_velocity(real_t v);

    ///
    /// \brief Set the linear and angular velocities
    /// of the vehicle
    ///
    void set_velocities(real_t v, real_t w);

    ///
    /// \brief Read current velocity of the vehicle
    ///
    real_t get_velocity()const{return 0.5*properties_.wheel_radius*(vr_ + vl_);}

    ///
    /// \brief get_right_wheel_velocity. Returns the right wheel velocity
    ///
    real_t get_right_wheel_velocity()const{return vr_;}

    ///
    /// \brief get_left_wheel_velocity. Returns the left wheel velocity
    ///
    real_t get_left_wheel_velocity()const{return vl_;}

    ///
    /// \brief Read the current angular velocity
    ///
    real_t get_w_velocity()const{return w_;/*(properties_.R*(vl_ - vr_))/(2.0*properties_.L);*/}

    ///
    /// \brief get_clipped_velocity. Retruns a velocity v in [vmin, vmax ]
    ///
    real_t get_clipped_velocity(real_t v)const;

    ///
    /// \brief get_clipped_angular_velocity. Retruns a velocity w in [wmin, wmax]
    ///
    real_t get_clipped_angular_velocity(real_t w)const;

    ///
    /// \brief get_bounding_box_type Returns the type of the bounding box
    ///
    kernel::geom::BBType get_bounding_box_type()const{return properties_.bbtype;}


private:

    ///
    /// \brief The properties of the robot
    ///
    DiffDriveConfig properties_;

    ///
    /// \brief The right wheel velocity
    ///
    real_t vr_;

    ///
    /// \brief The left wheel velocity
    ///
    real_t vl_;

    ///
    /// \brief The current angular velocity
    /// of the vehicle
    ///
    real_t w_;
};


}// vehicles
}

#endif // DIFD_DRIVE_VEHICLE_H
