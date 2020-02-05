#include "kernel/discretization/quad_element.h"
#include <exception>
namespace kernel
{
namespace numerics
{



template<int dim>
void
Quad<dim>::set_node(uint_t i, typename Quad<dim>::node_ptr_t node){

    if(!node){

        throw std::logic_error("Cannot add null node pointer");
    }

    if(i >= n_nodes()){
        throw std::logic_error("Invalid node index: " +
                               std::to_string(i) +
                               " not in [0," +
                               std::to_string(n_nodes()) +
                               ")");
    }

    this->nodes_[i] = node;
}


template<int dim>
void
Quad<dim>::append_node(typename Quad<dim>::node_ptr_t node){

    if(!node){

        throw std::logic_error("Cannot add null node pointer");
    }

    if( this->nodes_.size() == n_nodes()){
        throw std::logic_error("Nodes list is full cannot append new node ");
    }

    this->nodes_.push_back(node);
}


template<int dim>
void
Quad<dim>::reserve_nodes(uint n){

    if( n != n_nodes()){
        throw std::logic_error("Invalid number of nodes: " +
                               std::to_string(n) +
                               " not equal to" +
                               std::to_string(n_nodes()));
    }

    this->nodes_.reserve(n);
}

template<int dim>
typename Quad<dim>::node_ptr_t
Quad<dim>::get_node(uint_t n){

    if(n >= n_nodes()){
        throw std::logic_error("Invalid node index: " +
                               std::to_string(n) +
                               " not in [0," +
                               std::to_string(n_nodes()) +
                               ")");
    }

    return  this->nodes_[n];
}

template class Quad<2>;
//template class Quad<3>;

}
}
