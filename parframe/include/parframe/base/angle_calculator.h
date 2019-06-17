#ifndef ANGLE_CALCULATOR_H
#define ANGLE_CALCULATOR_H

#include "parframe/base/types.h"
#include <boost/noncopyable.hpp>

namespace kernel
{

class AngleCalculator: private boost::noncopyable
{
public:

    constexpr static real_t PI = 3.14159265359;

    /// \brief Convert degrees to rad
    constexpr static real_t deg_to_rad(real_t val){return val*(AngleCalculator::PI/180.0);}

    /// \brief Convert radians to degrees
    constexpr static real_t rad_to_deg(real_t val){return val*(180.0/AngleCalculator::PI);}

};


}

#endif // ANGLE_CALCULATOR_H
