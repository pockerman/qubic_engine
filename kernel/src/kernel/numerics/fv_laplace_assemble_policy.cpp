#include "kernel/numerics/fv_laplace_assemble_policy.h"
#include "kernel/numerics/fv_grad_base.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face_element.h"
#include "kernel/numerics/dof_manager.h"
#include "kernel/base/types.h"
#include "kernel/numerics/boundary_function_base.h"
#include "kernel/numerics/boundary_conditions_type.h"
#include "kernel/maths/functions/numeric_scalar_function.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"

#include "boost/any.hpp"
#include <exception>
#include <iostream>

namespace kernel{
namespace numerics{


template<int dim>
FVLaplaceAssemblyPolicy<dim>::FVLaplaceAssemblyPolicy()
    :
      fv_grads_(),
      elem_(nullptr),
      qvals_(),
      neigh_dofs_(),
      cell_dofs_(),
      dof_manager_(nullptr),
      boundary_func_(nullptr),
      rhs_func_(nullptr)
{}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::initialize_dofs_(){


    dof_manager_->get_dofs(*elem_, cell_dofs_);

    if(cell_dofs_.empty()){
        throw std::logic_error("Cell without dofs is used");
    }
    else if (cell_dofs_[0].id == KernelConsts::invalid_size_type()) {
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
            else if (tmp[0].id == KernelConsts::invalid_size_type()) {
                throw std::logic_error("Invalid DoF index");
            }

            neigh_dofs_.push_back(tmp[0]);
        }
    }
}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::compute_fluxes(){

    if(!fv_grads_){
        throw  std::logic_error("FV gradient pointer has not been set");
    }

    fv_grads_->compute_gradients(*elem_, fluxes_);
}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::reinit(const Element<dim>& element){

    elem_ = &element;
    initialize_dofs_();
    compute_fluxes();
}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::reinit(const Element<dim>& element, std::vector<real_t>&& qvals){
    elem_ = &element;
    qvals_ = qvals;
    qvals.clear();
    initialize_dofs_();
    compute_fluxes();
}

#ifdef USE_TRILINOS

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b )const{

    auto n_dofs = cell_dofs_.size() + neigh_dofs_.size();

    std::vector<real_t> row_entries(n_dofs, 0.0);
    std::vector<uint_t> row_dofs(n_dofs);

    // the indices of the faces local to the
    // elment that lie on the boundary
    std::vector<uint_t> boundary_faces;
    boundary_faces.reserve(elem_->n_faces());

    //the first dof is the row we work on
    row_dofs[0] = cell_dofs_[0].id;

    for(uint_t n=0; n<neigh_dofs_.size(); ++n){
      row_dofs[n+1] = neigh_dofs_[n].id;
    }

    //a dummy index to start counting from 1
    uint_t dummy_idx =1;

    for(uint_t f=0; f < elem_->n_faces(); ++f){

        const auto& face = elem_->get_face(f);

        if(!face.on_boundary()){
          //for every face we add to the diagonal entry
          real_t qval = qvals_.empty() ? 1.0 : qvals_[f];

          row_entries[0] += qval*fluxes_[f];
          row_entries[dummy_idx] = -qval*fluxes_[f];
          dummy_idx++;
        }
        else{

            boundary_faces.push_back(f);
        }
    }

    mat.set_entry(row_dofs[0], row_dofs[0], row_entries[0]);
    for(u_int idx=1; idx<row_dofs.size(); ++idx){
        mat.set_entry(row_dofs[0], row_dofs[idx], row_entries[idx]);
    }

    real_t rhs_val = 0.0;
    if(rhs_func_ != nullptr){
        rhs_val = rhs_func_->value(elem_->centroid());
    }

    b.add(cell_dofs_[0].id, rhs_val*elem_->volume());

    if(!boundary_faces.empty() && boundary_func_ != nullptr){
        apply_boundary_conditions(boundary_faces, mat, x, b);
    }
}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::apply_boundary_conditions(const  std::vector<uint_t>& bfaces, TrilinosEpetraMatrix& mat,
                                                        TrilinosEpetraVector& x, TrilinosEpetraVector& b )const{

        bool added=false;

        for(uint_t f=0; f<bfaces.size(); ++f){

            auto& face = elem_->get_face(bfaces[f]);

            //get the boundary condition type
            BCType type = boundary_func_->bc_type(face.boundary_indicator());

            if(type == BCType::DIRICHLET)
            {
                real_t bc_val = boundary_func_->value(face.centroid());
                uint_t var_dof = cell_dofs_[0].id;
                real_t qval = qvals_.empty() ? 1.0 : qvals_[bfaces[f]];

                //add the boundary condition type
                b.add(var_dof, bc_val*fluxes_[bfaces[f]]);
                auto flux_val = fluxes_[bfaces[f]];

                //add to the diagonal of the matrix
                mat.add_entry(var_dof, var_dof, qval*flux_val);

            }//Dirichlet
        }
}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::apply_boundary_conditions(const Mesh<dim>& mesh, TrilinosEpetraMatrix& mat,
                                                        TrilinosEpetraVector& x, TrilinosEpetraVector& b )const{


}


#endif

template class FVLaplaceAssemblyPolicy<1>;
template class FVLaplaceAssemblyPolicy<2>;
template class FVLaplaceAssemblyPolicy<3>;


}
}
