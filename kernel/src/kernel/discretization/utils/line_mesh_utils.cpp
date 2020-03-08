#include "kernel/discretization/utils/line_mesh_utils.h"
#include "kernel/discretization/line_mesh.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/edge_element.h"
#include "kernel/discretization/node.h"

#include <cmath>
namespace kernel{
namespace numerics{

const GeomPoint<2>
find_closest_point_to(const LineMesh<2>& mesh, const GeomPoint<2>&p){

    ConstElementMeshIterator<Active, LineMesh<2>> filter(mesh);
    auto begin = filter.begin();
    auto end   = filter.end();

    for(; begin != end; ++begin){

        auto element = *begin;

        auto v0 = element->get_vertex(0);
        auto v1 = element->get_vertex(1);

        real_t nom = std::fabs((v1[1] - v0[1])*p[0] - (v1[0] - v0[0])*p[1] + v1[0]*v0[1] - v1[1]*v0[0]);
        real_t denom = v0.distance(v1);
        real_t distance = nom / denom;
    }
}

}
}
