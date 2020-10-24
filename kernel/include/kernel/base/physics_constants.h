#ifndef PHYSICS_CONSTANTS_H
#define PHYSICS_CONSTANTS_H

#include "kernel/base/types.h"
#include "boost/noncopyable.hpp"

#include <string>

namespace kernel{

/// \brief PhysicsConsts collects various
/// physics constants
class PhysicsConsts: private boost::noncopyable
{
public:

    ///
    /// \brief PhysicsConsts. Constructor
    ///
    PhysicsConsts()=delete;

    ///
    /// \brief Returns the gravity constant.
    /// See: https://en.wikipedia.org/wiki/Gravitational_constant
    ///
    static real_t gravity_constant(){return 9.82;}

    ///
    /// \brief default_metric_system Returns the default metric system used
    /// \return
    ///
    static std::string default_metric_system(){return "SI";}

};
}

#endif // PHYSICS_CONSTANTS_H
