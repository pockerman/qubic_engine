#include "kernel/discretization/edge_element.h"

#include <exception>
namespace kernel
{

namespace numerics
{


EdgeElem<1>::EdgeElem(uint_t id, uint_t pid)
    :
      Element<1>(id, pid)
{}

void
EdgeElem<1>::set_neighbor(uint n, EdgeElem<1>::neighbor_ptr_t neigh){

    if(n >= n_faces()){
        throw std::logic_error("Invalid neighbor index "+std::to_string(n)+" not in [0, "+std::to_string(n_faces())+")");
    }

    this->neginbors_.push_back(neigh);
}

void
EdgeElem<1>::reserve_neighbors(uint n){

    if(n > n_faces() || n == 0){
        throw std::logic_error("Invalid neighbors number "+std::to_string(n)+" not equal to "+std::to_string(n_faces()));
    }

    this->neginbors_.reserve(n);
}

EdgeElem<1>::neighbor_ptr_t
EdgeElem<1>::get_neighbor(uint_t n){

    if(n >= n_faces()){
        throw std::logic_error("Invalid neighbor index "+std::to_string(n)+" not in [0, "+std::to_string(n_faces())+")");
    }

    if(this->neginbors_.empty()){
        throw std::logic_error("Neighbors list have not been initialized");
    }

    return this->neginbors_[n];
}

void
EdgeElem<1>::set_node(uint_t n, EdgeElem<1>::node_ptr_t node){

    if(n >= n_nodes()){
        throw std::logic_error("Invalid node index "+std::to_string(n)+" not in [0, "+std::to_string(n_nodes())+")");
    }

    this->nodes_.push_back(node);
}

EdgeElem<1>::node_ptr_t
EdgeElem<1>::get_node(uint_t n){

     if(n >= n_nodes()){
         throw std::logic_error("Invalid node index "+std::to_string(n)+" not in [0, "+std::to_string(n_nodes())+")");
     }

     if(this->nodes_.empty()){
         throw std::logic_error("Nodes list have not been initialized");
     }

     return this->nodes_[n];
 }

void
EdgeElem<1>::reserve_nodes(uint n){

    if(n > n_nodes() || n == 0){
        throw std::logic_error("Invalid number of nodes "+std::to_string(n)+" not equal to "+std::to_string(n_nodes()));
    }

    this->nodes_.reserve(n);
}
}
}
