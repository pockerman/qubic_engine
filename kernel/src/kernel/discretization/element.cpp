#include "kernel/discretization/element.h"

namespace kernel
{

namespace numerics
{

template<int dim>
Element<dim>::Element(uint_t id, uint_t pid)
    :
      MeshEntity(id, pid),
      dofs_()
{}

template<int dim>
Element<dim>::~Element()
{}

template<int dim>
void
Element<dim>::invalidate_dofs(const std::string_view name){

    dofs_.invalidate_dofs(name);
}


template<int dim>
void
Element<dim>::insert_dof(DoF&& dof){
    dofs_.insert_dof(std::move(dof));
}

template<int dim>
typename Element<dim>::neighbor_ptr_t
Element<dim>::neighbor_ptr(uint_t n){
    return neginbors_[n];
}

template<int dim>
const typename Element<dim>::neighbor_ptr_t
Element<dim>::neighbor_ptr(uint_t n)const{
    return neginbors_[n];
}


template class Element<1>;
template class Element<2>;
template class Element<3>;

}

}
