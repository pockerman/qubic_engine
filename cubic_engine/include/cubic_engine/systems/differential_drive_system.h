#ifndef DIFFERENTIAL_DRIVE_SYSTEM_H
#define DIFFERENTIAL_DRIVE_SYSTEM_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <map>
#include <string>

namespace cengine
{

class DiffDriveSystem
{

public:

    struct State;
    typedef State state_type;

    /// \brief Structure that describes the state of the system
    struct State
    {
      DynVec<real_t> position;
      DynVec<real_t> velocity;
      real_t angular_velocity;
      real_t yaw;
      DynVec<real_t> wheels_velocities;

      State()=default;
    };

    /// \brief Constructor
    DiffDriveSystem()=default;

    /// \brief Set the scalar property
    void set_scalar_property(const std::string& name, real_t value);

    /// \brief Return the value of the specified scalar property.
    /// Throws std::invalid_argument is name not in underlying map
    real_t get_scalar_property(const std::string& name)const;


private:

    /// \brief The state of the system
    State state_;

    /// \brief Scalar properties of the system
    std::map<std::string, real_t> scalar_properties_;
};


}

#endif // DIFFERENTIAL_DRIVE_SYSTEM_H
