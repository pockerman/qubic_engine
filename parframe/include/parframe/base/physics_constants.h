#ifndef PHYSICS_CONSTANTS_H
#define PHYSICS_CONSTANTS_H

#include "parframe/base/types.h"
#include "boost/noncopyable.hpp"

namespace kernel
{


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
