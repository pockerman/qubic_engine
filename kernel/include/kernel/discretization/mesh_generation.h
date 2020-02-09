#ifndef MESH_GENERATION_H
#define MESH_GENERATION_H

#include "kernel/base/types.h"

namespace kernel
{

// forward declarations
template<int dim, typename T> class GeomPoint;

namespace numerics
{

/// forward declarations
class LineMesh;

/// \brief Generate a LineMesh with n_elements
void build_mesh(LineMesh& mesh, uint_t n_elements,
                const GeomPoint<1, real_t>& start, const GeomPoint<1, real_t>& end );

}

}

#endif // MESH_GENERATION_H
