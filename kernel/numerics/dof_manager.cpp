#include "kernel/numerics/dof_manager.h"
#include "kernel/numerics/scalar_variable.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/mesh.h"

namespace kernel{
namespace numerics {

template<int dim>
FVDoFManager<dim>::FVDoFManager()
    :
      n_dofs_(0),
      var_name_("")
{}

template<int dim>
FVDoFManager<dim>::~FVDoFManager()
{}


template<int dim>
void
FVDoFManager<dim>::distribute_dofs(Mesh<dim>& mesh, const ScalarVar& var){

    n_dofs_ = 0;
    var_name_ = var.name();

    invalidate_dofs(mesh);
    do_distribute_dofs(mesh);
}

template<int dim>
void
FVDoFManager<dim>::invalidate_dofs(Mesh<dim>& mesh){

    n_dofs_ = 0;
    ElementMeshIterator<Active, Mesh<dim>> filter(mesh);
    auto begin = filter.begin();
    auto end = filter.end();

    for(; begin != end; begin++){

        auto* elem = *begin;
        elem->invalidate_dofs(var_name_);
    }
}

template<int dim>
void
FVDoFManager<dim>::do_distribute_dofs(Mesh<dim>& mesh){

    uint_t next_available_id = 0;

    ElementMeshIterator<Active, Mesh<dim>> filter(mesh);
    auto begin = filter.begin();
    auto end = filter.end();

    for(; begin != end; begin++){

        auto* elem = *begin;
        elem->insert_dof({var_name_, next_available_id++, true});
    }

    // set the dofs distributed
    n_dofs_ = next_available_id;
}

template<int dim>
void
FVDoFManager<dim>::get_dofs(const Element<dim>& elem, std::vector<DoF>& dofs)const{

    elem.get_dofs(var_name_, dofs);
}

template class FVDoFManager<1>;
template class FVDoFManager<2>;
template class FVDoFManager<3>;

}
}
