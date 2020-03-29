#ifndef UNIT_CONVERTER_H
#define UNIT_CONVERTER_H

#include "kernel/base/types.h"
#include "kernel/maths/constants.h"
#include <boost/noncopyable.hpp>

namespace kernel {


/// \brief UnitConverter
class UnitConverter:private boost::noncopyable
{

public:

    UnitConverter()=delete;

    /// \brief Convert RPM to angular speed in m/s
    static real_t rpm_to_angular_speed(uint_t rpm){return (2.0*MathConsts::PI*rpm)/60.0;}

    /// \brief Convert RPM to angular speed in m/s
    static real_t angular_speed_to_linear_speed(real_t r, real_t w){return r*w;}

};
}

#endif // UNIT_CONVERTER_H
