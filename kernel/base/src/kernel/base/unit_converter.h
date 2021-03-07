#ifndef UNIT_CONVERTER_H
#define UNIT_CONVERTER_H

#include "kernel/base/types.h"
#include "kernel/maths/constants.h"
#include <boost/noncopyable.hpp>

namespace kernel {

///
/// \brief UnitConverter. Helper class that handles
/// unit conversions
///
class UnitConverter:private boost::noncopyable
{

public:

    ///
    /// \brief UnitConverter. Constructor deleted
    ///
    UnitConverter()=delete;

    ///
    /// \brief Convert RPM to angular speed in m/s
    ///
    static real_t rpm_to_angular_speed(uint_t rpm){return (2.0*MathConsts::PI*rpm)/60.0;}

    ///
    /// \brief Convert RPM to angular speed in m/s
    ///
    static real_t angular_speed_to_linear_speed(real_t r, real_t w){return r*w;}

    ///
    /// \brief Convert radians to degrees
    ///
    static real_t rad_to_degrees(real_t rad){return (rad*180.0)/MathConsts::PI;}

    ///
    /// \brief Convert degrees to radians
    ///
    static real_t degrees_to_rad(real_t degs){return (degs*MathConsts::PI)/180.0;}

};
}

#endif // UNIT_CONVERTER_H
