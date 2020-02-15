#ifndef SERIAL_GRAPH_BUILDER_H
#define SERIAL_GRAPH_BUILDER_H

#include "kernel/data_structs/boost_serial_graph.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"

namespace kernel{

/// \brief Build a BoostSerialGraph from a
/// given kernel::numerics::Mesh object
template<int dim, typename VertexData,typename EdgeData>
void build_mesh_graph(const numerics::Mesh<dim>& mesh, BoostSerialGraph<VertexData, EdgeData>& graph){

    using numerics::Mesh;
    using numerics::Active;

    numerics::ConstElementMeshIterator<Active, Mesh<dim>> filter(mesh);

    auto begin = filter.begin();
    auto end = filter.end();

    for(; begin != end; ++begin){

        auto* element = *begin;

        uint_t v_id = element->get_id();

        // add a vertex
        auto& vertex = graph.add_vertex(VertexData());
        vertex.id = v_id;
    }

    // loop once more to build the connectivity
    begin = filter.begin();

    for(; begin != end; ++begin){

        auto* element = *begin;

        uint_t v_id = element->get_id();

        // now add edges to the neighbors
        for(uint_t n=0; n<element->n_neighbors(); ++n){

            auto* neigh = element->neighbor_ptr(n);

            if(neigh){
               graph.add_edge(v_id, neigh->get_id());
            }
        }
    }

}

}

#endif // SERIAL_GRAPH_BUILDER_H
