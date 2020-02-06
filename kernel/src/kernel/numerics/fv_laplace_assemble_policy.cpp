#include "kernel/numerics/fv_laplace_assemble_policy.h"
#include "kernel/numerics/fv_grad_base.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face_element.h"
#include "kernel/numerics/dof_manager.h"
#include "kernel/base/types.h"
#include "kernel/numerics/boundary_function_base.h"
#include "kernel/numerics/boundary_conditions_type.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"

#include "boost/any.hpp"
#include <exception>

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
      boundary_func_(nullptr)
{}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::initialize_dofs_(){


    dof_manager_->get_dofs(*elem_, cell_dofs_);

    neigh_dofs_.reserve(elem_->n_neighbors());
    std::vector<DoF> tmp;

    // get the dofs off the neighbors
    for(uint_t neigh=0; neigh<elem_->n_neighbors(); ++neigh){

        auto* neigh_elem = elem_->neighbor_ptr(neigh);

        if(neigh_elem){
            dof_manager_->get_dofs(*neigh_elem, tmp);
            neigh_dofs_.push_back(tmp[0]);
        }
    }
}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::compute_fluxes(){

    fluxes_.resize(elem_->n_faces(), 0.0);
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

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b )const{

    auto n_dofs = cell_dofs_.size() + neigh_dofs_.size();

    std::vector<real_t> row_entries(n_dofs, 0.0);
    std::vector<uint_t> row_dofs(n_dofs);

    std::vector<boost::any> boundary_faces;
    boundary_faces.reserve(elem_->n_faces());

    //the first dof is the row we work on
    row_dofs[0] = cell_dofs_[0].id;

    for(uint_t n=0; n<neigh_dofs_.size(); ++n){
      row_dofs[n+1] = neigh_dofs_[n].id;
    }

    //a dummy index to start counting from 1
    uint_t dummy_idx =1;

    for(uint_t f=0; elem_->n_faces(); ++f){

        const auto& face = elem_->get_face(f);

        if(!face.on_boundary())
        {
          //for every face we add to the diagonal entry
          real_t qval = qvals_.empty()?1.0:qvals_[f];

          row_entries[0] += qval*fluxes_[f];
          row_entries[dummy_idx] = -qval*fluxes_[f];
          dummy_idx++;
        }
        else{
            boundary_faces.push_back(face);
        }
    }

    mat.add_row_entries(row_dofs, row_entries);

    if(!boundary_faces.empty()){
        apply_boundary_conditions(boundary_faces, mat, x, b);
    }
}

template<int dim>
void
FVLaplaceAssemblyPolicy<dim>::apply_boundary_conditions(const  std::vector<boost::any>& bfaces, TrilinosEpetraMatrix& mat,
                                                        TrilinosEpetraVector& x, TrilinosEpetraVector& b )const{


        for(uint_t f=0; f<bfaces.size(); ++f){

            auto& face = boost::any_cast<typename Element<dim>::cface_ref_t>(bfaces[f]);

            //get the boundary condition type
            BCType type = boundary_func_->bc_type(face.boundary_indicator());

            if(type == BCType::DIRICHLET)
            {
                real_t bc_val = boundary_func_->value(face.centroid());
                uint_t var_dof = cell_dofs_[0].id;
                real_t qval = qvals_.empty() ? 1.0 : qvals_[f];

                //add the boundary condition type
                b.add(var_dof, bc_val*fluxes_[f]);

                //add to the diagonal of the matrix
                mat.add_entry(var_dof, var_dof, qval*fluxes_[f]);

            }//Dirichlet
        }
}

template class FVLaplaceAssemblyPolicy<1>;
template class FVLaplaceAssemblyPolicy<2>;
template class FVLaplaceAssemblyPolicy<3>;


}
}
