#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "kernel/base/types.h"
#include "kernel/geometry/geom_point.h"

namespace kernel{

const GeomPoint<3>
cross_product(const GeomPoint<3>& o1, const GeomPoint<3>& o2);

const GeomPoint<3>
cross_product(const GeomPoint<2>& o1, const GeomPoint<2>& o2);

}

#endif // GEOMETRY_UTILS_H
