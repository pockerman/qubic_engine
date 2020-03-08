#ifndef LINE_MESH_UTILS_H
#define LINE_MESH_UTILS_H

#include "kernel/geometry/geom_point.h"

namespace kernel {
namespace numerics {

template<int dim> class LineMesh;

/// \brief Find the closest point on the given LineMesh
/// The function loops over the elements in the
/// mesh and uses the calculation described in
/// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
/// It then out of the calculated points it picks the one with the smallest
/// distance
const GeomPoint<2> find_closest_point_to(const LineMesh<2>& mesh, const GeomPoint<2>&p);

}

}

#endif // LINE_MESH_UTILS_H
