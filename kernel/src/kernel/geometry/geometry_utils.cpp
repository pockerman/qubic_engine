#include "kernel/geometry/geometry_utils.h"

namespace kernel{
const GeomPoint<3>
cross_product(const GeomPoint<3>& o1, const GeomPoint<3>& o2)
{


 // |     i          j          k    |
 // |(*o1)(0)    (*o2)(1)    (*o3)(2)|
 // |   p(0)       p(1)       p(2)   |

  std::vector<real_t> vals(3);
  auto data1 = o1.coordinates();
  auto data2 = o2.coordinates();

  //from mathworld.wolfram.com/CrossProduct.html
  vals[0] = data1[1]*data2[2] - data1[2]*data2[1];
  vals[1] = -data1[0]*data2[2] + data1[2]*data2[0];
  vals[2] = data1[0]*data2[1] - data1[1]*data2[0];

  return GeomPoint<3>(vals);

}

const GeomPoint<3>
cross_product(const GeomPoint<2>& o1, const GeomPoint<2>& o2)
{

    // |     i          j          k    |
    // |(*o1)(0)    (*o2)(1)    (*o3)(2)|
    // |   p(0)       p(1)       p(2)   |

    std::vector<real_t> vals(3);
    auto data1 = o1.coordinates();
    auto data2 = o2.coordinates();

    //from mathworld.wolfram.com/CrossProduct.html
    vals[0] = data1[1]*0.0 - 0.0*data2[1];
    vals[1] = -data1[0]*0.0 + 0.0*data2[0];
    vals[2] = data1[0]*data2[1] - data1[1]*data2[0];

   return GeomPoint<3>(vals);
}

std::pair<bool, real_t> has_intersection(real_t discriminant,
                                         real_t b, real_t a){

    std::pair<bool, real_t> result = {false, 0.0};

    /// no intersection
    if( discriminant < 0. )
        return result;

    /// ray didn't totally miss sphere,
    /// so there is a solution to
    /// the equation.

    discriminant = std::sqrt( discriminant );

    /// either solution may be on or off the ray so need to test both
    /// t1 is always the smaller value, because BOTH discriminant and
    /// a are nonnegative.
    auto t1 = (-b - discriminant)/(2*a);
    auto t2 = (-b + discriminant)/(2*a);

    /// 3x HIT cases:
    ///          -o->             --|-->  |            |  --|->
    /// Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit),

    /// 3x MISS cases:
    ///       ->  o                     o ->              | -> |
    /// FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

    if( t1 >= 0 && t1 <= 1 ){

      /// t1 is the intersection, and it's closer than t2
      /// (since t1 uses -b - discriminant)
      /// Impale, Poke
        return {true, t1} ;
    }

    /// here t1 didn't intersect so we are either started
    /// inside the sphere or completely past it
    if( t2 >= 0 && t2 <= 1 ){
      /// ExitWound
      return {true, t2} ;
    }

    /// no intn: FallShort, Past, CompletelyInside
    return result ;
}



}
