#include "kernel/numerics/simple_fv_time_stepper.h"
#include "kernel/numerics/dof_manager.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/element_mesh_iterator.h"

#ifdef USE_TRILINOS
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#endif

#include <exception>
#include <iostream>


namespace kernel{
namespace numerics {
template<int dim>
SimpleFVTimeAssemblyPolicy<dim>::SimpleFVTimeAssemblyPolicy()
    :
      dt_(0.1),
      nsols_(0),
      elem_(nullptr),
      neigh_dofs_(),
      cell_dofs_(),
      dof_manager_(nullptr),
      m_ptr_(nullptr)
{}

template<int dim>
void
SimpleFVTimeAssemblyPolicy<dim>::initialize_dofs_(){


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

template<int dim>
void
SimpleFVTimeAssemblyPolicy<dim>::reinit(const Element<dim>& element){

    elem_ = &element;
    initialize_dofs_();
}

#ifdef USE_TRILINOS

template<int dim>
void
SimpleFVTimeAssemblyPolicy<dim>::assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x,
                                          TrilinosEpetraVector& b, const std::vector<TrilinosEpetraVector>& old_solutions ){

    // loop over the elements
    ConstElementMeshIterator<Active, Mesh<dim>> filter(*m_ptr_);

    auto elem_itr = filter.begin();
    auto elem_itr_e = filter.end();

    for(; elem_itr != elem_itr_e; ++elem_itr){

        auto* elem = *elem_itr;

        reinit(*elem);
        assemble_one_element(mat, x, b, old_solutions);
    }
}

template<int dim>
void
SimpleFVTimeAssemblyPolicy<dim>::assemble_one_element(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x,
                                                      TrilinosEpetraVector& b, const std::vector<TrilinosEpetraVector>& old_solutions ){

    auto n_dofs = cell_dofs_.size();

    std::vector<real_t> row_entries(n_dofs, 0.0);

    mat.set_entry(cell_dofs_[0].id, cell_dofs_[0].id, elem_->volume()/dt_);
    auto old_sol = old_solutions[0][cell_dofs_[0].id];
    b.add(cell_dofs_[0].id, (old_sol*elem_->volume())/dt_);
}

#endif

template class SimpleFVTimeAssemblyPolicy<1>;
template class SimpleFVTimeAssemblyPolicy<2>;
template class SimpleFVTimeAssemblyPolicy<3>;
}
}
