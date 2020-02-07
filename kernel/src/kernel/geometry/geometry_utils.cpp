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
}
