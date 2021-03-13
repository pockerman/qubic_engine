#ifndef CIRCLE_H
#define CIRCLE_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/maths/constants.h"
#include <cmath>

namespace kernel{

/// \brief The Circle class. Models a common circle
class Circle
{
public:

    /// \brief Construct a circle centerd at the origin
    Circle(real_t r);

    /// \brief Construct a circle given its radius and center
    Circle(real_t r, const GeomPoint<2>& center);

    /// \brief Returns the radius of the circle
    real_t radius()const{return r_;}

    /// \brief Returns the center of the circle
    GeomPoint<2> center()const{return center_;}

    /// \brief Returns the area
    real_t area()const;

    /// \brief Returns true if the given point lies inside the circle
    bool is_inside(const GeomPoint<2>& point, real_t tol = KernelConsts::tolerance())const;

private:

    /// \brief The radius of the circle
    real_t r_;


    /// \brief The center of the circle
    GeomPoint<2> center_;

};

inline
Circle::Circle(real_t r)
    :
      r_(r),
      center_(0.0)
{}


inline
Circle::Circle(real_t r, const GeomPoint<2>& center)
    :
   r_(r),
   center_(center)
{}


inline
real_t
Circle::area()const{

    return MathConsts::PI*r_*r_;

}

inline
bool
Circle::is_inside(const GeomPoint<2>& point, real_t tol)const{

    if(std::pow((center_[0] - point[0]), 2) + std::pow((center_[1] - point[1]), 2) - r_*r_ < tol){
        return true;
    }

    return false;

}


}

#endif // CIRCLE_H
