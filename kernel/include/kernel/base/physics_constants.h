#ifndef PHYSICS_CONSTANTS_H
#define PHYSICS_CONSTANTS_H

#include "kernel/base/types.h"
#include "boost/noncopyable.hpp"

namespace kernel{

/// \brief PhysicsConsts collects various
/// physics constants
class PhysicsConsts: private boost::noncopyable
{
public:

    PhysicsConsts()=delete;

    /// \brief Returns the gravity constant.
    /// See: https://en.wikipedia.org/wiki/Gravitational_constant
    static real_t gravity_constant(){return 9.82;}

};
}

#endif // PHYSICS_CONSTANTS_H
