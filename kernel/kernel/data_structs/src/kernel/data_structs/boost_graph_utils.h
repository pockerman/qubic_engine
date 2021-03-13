#ifndef BOOST_GRAPH_UTILS_H
#define BOOST_GRAPH_UTILS_H

#include "kernel/base/kernel_consts.h"

namespace kernel {

template<typename GraphType, typename Predicate>
uint_t
find_vertex(const GraphType& graph, const Predicate& pred){

    for(uint_t v=0; v<graph.n_vertices(); ++v){
        const auto& vertex = graph.get_vertex(v);
        if(pred(vertex)){
            return vertex.id;
        }
    }

    return KernelConsts::invalid_size_type();
}

}

#endif // BOOST_GRAPH_UTILS_H
