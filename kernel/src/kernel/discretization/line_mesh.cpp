#include "kernel/discretization/line_mesh.h"
#include "kernel/discretization/edge_element.h"

namespace kernel
{

namespace numerics
{


uint_t
LineMesh::n_edges()const{
    return 0;
}


uint_t
LineMesh::n_faces()const{
    return 0;
}


LineMesh::node_ptr_t
LineMesh::add_node(real_t x){
    return nullptr;
}


LineMesh::elem_ptr_t
LineMesh::add_element(){

    uint_t id = elements_.size();
    auto* elemptr = new EdgeElem<1>(id);
    elements_.push_back(elemptr);
    return elemptr;
}


}
}
