#include "kernel/numerics/fvm/fv_assemble_policy.h"
#include "kernel/numerics/dof_manager.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/mesh.h"


namespace kernel{
namespace numerics{


template<int dim>
FVAssemblyPolicy<dim>::FVAssemblyPolicy()
    :
      elem_(nullptr),
      qvals_(),
      neigh_dofs_(),
      cell_dofs_(),
      dof_manager_(nullptr),
      boundary_func_(nullptr),
      rhs_func_(nullptr),
      volume_func_(nullptr),
      m_ptr_(nullptr)
{}

template<int dim>
void
FVAssemblyPolicy<dim>::initialize_dofs_(){


    dof_manager_->get_dofs(*elem_, cell_dofs_);

    if(cell_dofs_.empty()){
        throw std::logic_error("Cell without dofs is used");
    }

    if (cell_dofs_[0].id == KernelConsts::invalid_size_type()) {
        throw std::logic_error("Invalid DoF index");
    }

    neigh_dofs_.clear();
    neigh_dofs_.reserve(elem_->n_neighbors());


    // get the dofs off the neighbors
    for(uint_t neigh=0; neigh<elem_->n_neighbors(); ++neigh){

        auto* neigh_elem = elem_->neighbor_ptr(neigh);

        if(neigh_elem){

            std::vector<DoF> tmp;

            dof_manager_->get_dofs(*neigh_elem, tmp);

            if(tmp.empty()){
                throw std::logic_error("Cell without dofs is used");
            }

            if (tmp[0].id == KernelConsts::invalid_size_type()) {
                throw std::logic_error("Invalid DoF index");
            }

            neigh_dofs_.push_back(tmp[0]);
        }
    }
}


template class FVAssemblyPolicy<1>;
template class FVAssemblyPolicy<2>;
template class FVAssemblyPolicy<3>;

}
}
