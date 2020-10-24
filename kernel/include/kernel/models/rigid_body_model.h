#ifndef RIGID_BODY_MODEL_H
#define RIGID_BODY_MODEL_H

#include "kernel/base/types.h"
#include <string>

namespace kernel {
namespace models {

///
/// \brief RigidBodyModel class. Base class for deriving
/// rigid body models
///
template<typename ChassisTp, typename DynamicsTp>
class RigidBodyModel
{
public:

    ///
    /// \brief chassis_t The type of the chassis
    ///
    typedef ChassisTp chassis_t;

    ///
    /// \brief moment_t The inertial moment type
    ///
    typedef typename chassis_t::inertia_moment_t inertia_moment_t;

    ///
    /// \brief forces_t The type of the forces acting on the chassis
    ///
    typedef typename chassis_t::force_t chassis_force_t;

    ///
    /// \brief dynamics_t The type of the dynamics
    ///
    typedef DynamicsTp dynamics_t;

    ///
    /// \brief RigidBodyModel. Constructor
    ///
    RigidBodyModel();

    ///
    /// \brief load_chassis_json. Load the chassis
    /// description from json file
    ///
    void load_chassis_json(const std::string& filename){chassis_.load_from_json(filename);}

    ///
    /// \brief load_dynamics_json. Load the dynamics
    /// description from json file
    ///
    void load_dynamics_json(const std::string& filename){dynamics_.load_from_json(filename);}

    ///
    /// \brief get_chassis_forces Returns the sum of
    /// the forces acting on the chassis
    ///
    chassis_force_t get_chassis_forces()const{return chassis_.compute_forces();}

private:

    ///
    /// \brief chassis_ The chassis description of the rigid body
    ///
    chassis_t chassis_;

    ///
    /// \brief dynamics_ The dynamics description of the rigid body
    ///
    dynamics_t dynamics_;

};

template<typename ChassisTp, typename DynamicsTp>
RigidBodyModel<ChassisTp, DynamicsTp>::RigidBodyModel()
    :
     chassis_(),
     dynamics_()
{}

}

}

#endif // RIGID_BODY_MODEL_H
