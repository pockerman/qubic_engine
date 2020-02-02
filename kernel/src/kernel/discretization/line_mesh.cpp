#include "kernel/discretization/line_mesh.h"
#include "kernel/discretization/edge_element.h"
#include "kernel/discretization/node.h"

#include <exception>
namespace kernel
{

namespace numerics
{


void
LineMesh::reserve_elements(uint_t elements){

    if(elements == 0){

        throw std::logic_error("Cannot reserve space for zero elements");
    }

    elements_.reserve(elements);
}

void
LineMesh::reserve_nodes(uint_t nodes){

    if(nodes == 0){
       throw std::logic_error("Cannot reserve space for zero nodes");
    }

    nodes_.reserve(nodes);
}

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
    uint_t id = nodes_.size();
    nodes_.push_back(std::make_shared<Node<1>>(x, id));
    return nodes_[id];
}

LineMesh::node_ptr_t
LineMesh::get_node(uint_t n){
    if(n >= nodes_.size()){
       throw std::logic_error("Invalid node index "+std::to_string(n)+" not in [0,"+std::to_string(nodes_.size())+")");
    }

    return nodes_[n];
}


LineMesh::elem_ptr_t
LineMesh::add_element(){

    uint_t id = elements_.size();
    elements_.push_back(std::make_shared<EdgeElem<1>>(id));
    return elements_[id];
}

LineMesh::elem_ptr_t
LineMesh::get_elem(uint_t e){

    if(e >= elements_.size()){
       throw std::logic_error("Invalid element index "+std::to_string(e)+" not in [0,"+std::to_string(elements_.size())+")");
    }

    return elements_[e];
}


}
}
