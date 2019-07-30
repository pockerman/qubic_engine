#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <map>
#include <string>
namespace cengine
{

/// \brief Implements a simple PID control
/// for a scalar variable
class PIDControl
{
public:

    /// \brief Constructor
    PIDControl(real_t Kp, real_t Kd=0.0, real_t Ki=0.0);

    /// \brief calcaulate the control to apply
    real_t execute(real_t error, real_t dt=0.0);

    /// \brief Set the parameter value
    void set_paramter(const std::string& name, real_t val);

    /// \brief Set the accummulated error
    void set_error(real_t err){error_ = err;}

private:

    std::map<std::string, real_t> parameters_;
    real_t error_;


};
}

#endif // PID_CONTROLLER_H
