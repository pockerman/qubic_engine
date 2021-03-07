#ifndef FV_LAPLACE_ASSEMBLE_POLICY_THREADED_H
#define FV_LAPLACE_ASSEMBLE_POLICY_THREADED_H

#include "kernel/base/types.h"
#include "kernel/base/config.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face_element.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/mesh.h"
#include "kernel/numerics/dof.h"
#include "kernel/numerics/dof_manager.h"
#include "kernel/numerics/boundary_conditions_type.h"
#include "kernel/numerics/boundary_function_base.h"
#include "kernel/numerics/fvm/fv_grad_base.h"
#include "kernel/maths/functions/numeric_scalar_function.h"

#include "kernel/parallel/threading/simple_task.h"

#ifdef USE_TRILINOS
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#endif
#include <vector>
#include <string>
#include <memory>


namespace kernel {
namespace numerics {

/// forward declarations
template<int dim> class FVGradBase;
template<int dim> class Element;
template<int dim> class Mesh;
template<int dim> class FVDoFManager;
template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;

#ifdef USE_TRILINOS
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;
#endif

/// \brief Policy that assembles Laplaces terms
/// on a user specified mesh using user specified
/// gradient scheme

template<int dim, typename Executor>
class FVLaplaceAssemblyPolicyThreaded
{
public:

    /// \brief The executor of the policy
    typedef Executor executot_t;

    /// \brief Constructor
    FVLaplaceAssemblyPolicyThreaded();

#ifdef USE_TRILINOS

    /// \brief Assemble the data
    void assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b );

#endif

    /// \brief Set the function that describes the boundary conditions
    void set_boundary_function(const BoundaryFunctionBase<dim>& func){boundary_func_ = &func;}

    /// \brief Set the function that describes the boundary conditions
    void set_rhs_function(const NumericScalarFunction<dim>& func){rhs_func_ = &func;}

    /// \brief Set the function that describes the boundary conditions
    void set_volume_term_function(const NumericScalarFunction<dim>& func){volume_func_ = &func;}

    /// \brief Set the object that describes the dofs
    void set_dof_manager(const FVDoFManager<dim>& dof_manager){dof_manager_ = &dof_manager;}

    /// \brief Set the mesh pointer
    void set_mesh(const Mesh<dim>& mesh){m_ptr_ = &mesh;}

    /// \brief Set the executor
    void set_executor(executot_t& executor){executor_ = &executor;}

    /// \brief Build the  gradient scheme
    template<typename Factory>
    void build_gradient(const Factory& factory);

private:

    template<typename MatrixTp, typename VectorTp>
    struct AssembleTask;

    /// \brief Pointer to the FV gradient approximation
    std::shared_ptr<FVGradBase<dim>> fv_grads_;

    /// \brief The DoFManager that handles the dofs
    const FVDoFManager<dim>* dof_manager_;

    /// \brief The boundary function used
    const BoundaryFunctionBase<dim>* boundary_func_;

    /// \brief Pointer to the function object that describes the
    /// rhs
    const NumericScalarFunction<dim>* rhs_func_;

    /// \brief Pointer to the function object that describes the
    /// any volume terms to assemble
    const NumericScalarFunction<dim>* volume_func_;

    /// \brief The Mesh over which the policy is working
    const Mesh<dim>* m_ptr_;

    /// \brief The tasks
    std::vector<std::unique_ptr<TaskBase>> tasks_;

    /// \brief Pointer to the executor
    executot_t* executor_;
};

template<int dim, typename Executor>
template<typename Factory>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::build_gradient(const Factory& factory){

    fv_grads_ = factory();
}

template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
struct FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask: public SimpleTaskBase<Null>
{

public:

    typedef MatrixTp matrix_t;
    typedef VectorTp vector_t;

    /// \brief Constructor
    AssembleTask(uint_t t, matrix_t& mat, vector_t& b, vector_t& x,
                 std::shared_ptr<FVGradBase<dim>> fv_grads, const FVDoFManager<dim>& dof_manager,
                 const Mesh<dim>& mesh,
                 const BoundaryFunctionBase<dim>* boundary_func,
                 const NumericScalarFunction<dim>* rhs_func,
                 const NumericScalarFunction<dim>* vol_func);

    /// \brief Compute the fluxes over the cell last
    /// reinitialized
    void compute_fluxes();

    /// \brief Apply the boundary conditions
    void apply_boundary_conditions(const  std::vector<uint_t>& bfaces );

    /// \brief assemble one element contribution
    void assemble_one_element();

    /// \brief Reinitialize the policy
    void reinit(const Element<dim>& element);

    /// \brief initialize dofs
    void initialize_dofs();

protected:

    virtual void run()override final;

    /// \brief The matrix to assemble
    MatrixTp& mat_;

    /// \brief The rhs
    VectorTp& b_;

    /// \brief The solution
    VectorTp& x_;

    /// \brief Pointer to the FV gradient approximation
    std::shared_ptr<FVGradBase<dim>> fv_grads_;

    /// \brief The DoFManager that handles the dofs
    const FVDoFManager<dim>* dof_manager_;

    /// \brief The boundary function used
    const BoundaryFunctionBase<dim>* boundary_func_;

    /// \brief Pointer to the function object that describes the
    /// rhs
    const NumericScalarFunction<dim>* rhs_func_;

    /// \brief Pointer to the function object that describes the
    /// any volume terms to assemble
    const NumericScalarFunction<dim>* volume_func_;

    /// \brief The Mesh over which the policy is working
    const Mesh<dim>* m_ptr_;

    /// \brief The element over which the policy is working
    const Element<dim>* elem_;

    /// \brief The values over the cell sides
    std::vector<real_t> qvals_;

    /// \brief The dofs of the neighbors of the cell we do work on
    std::vector<DoF> neigh_dofs_;

    /// \brief The dofs of the cell
    std::vector<DoF> cell_dofs_;

    /// \brief The cell fluxes
    std::vector<real_t> fluxes_;

};

template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<MatrixTp,VectorTp>::AssembleTask(uint_t t, matrix_t& mat, vector_t& b, vector_t& x,
                                                                                              std::shared_ptr<FVGradBase<dim>> fv_grads, const FVDoFManager<dim>& dof_manager,
                                                                                              const Mesh<dim>& mesh,
                                                                                              const BoundaryFunctionBase<dim>* boundary_func,
                                                                                              const NumericScalarFunction<dim>* rhs_func,
                                                                                              const NumericScalarFunction<dim>* vol_func)
    :
    SimpleTaskBase<Null>(t),
    mat_(mat),
    b_(b),
    x_(x),
    fv_grads_(fv_grads),
    dof_manager_(&dof_manager),
    m_ptr_(&mesh),
    boundary_func_(boundary_func),
    rhs_func_(rhs_func),
    volume_func_(vol_func),
    elem_(nullptr),
    qvals_(),
    neigh_dofs_(),
    cell_dofs_(),
    fluxes_()
{}


template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<MatrixTp,VectorTp>::run(){

    // loop over the elements
    ConstElementMeshIterator<ActiveOnProc, Mesh<dim>> filter(*m_ptr_);

    auto elem_itr = filter.begin(this->get_id());
    auto elem_itr_e = filter.end(this->get_id());

    for(; elem_itr != elem_itr_e; ++elem_itr){

        auto* elem = *elem_itr;

        reinit(*elem);
        assemble_one_element();
    }
}

template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<MatrixTp,VectorTp>::reinit(const Element<dim>& element){

    elem_ = &element;
    initialize_dofs();
    compute_fluxes();
}

template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<MatrixTp,VectorTp>::initialize_dofs(){


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


template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<MatrixTp,VectorTp>::compute_fluxes(){

    if(!fv_grads_){
        throw  std::logic_error("FV gradient pointer has not been set");
    }

    fv_grads_->compute_gradients(*elem_, fluxes_);
}

template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<MatrixTp,VectorTp>::apply_boundary_conditions(const  std::vector<uint_t>& bfaces ){

    bool added=false;

    for(uint_t f=0; f<bfaces.size(); ++f){

        auto& face = elem_->get_face(bfaces[f]);

        //get the boundary condition type
        BCType type = boundary_func_->bc_type(face.boundary_indicator());

        if(type == BCType::DIRICHLET || type == BCType::ZERO_DIRICHLET)
        {
            real_t bc_val = boundary_func_->value(face.centroid());
            uint_t var_dof = cell_dofs_[0].id;
            real_t qval = qvals_.empty() ? 1.0 : qvals_[bfaces[f]];

            //add the boundary condition type
            b_.add(var_dof, bc_val*fluxes_[bfaces[f]]);
            auto flux_val = fluxes_[bfaces[f]];

            //add to the diagonal of the matrix
            mat_.add_entry(var_dof, var_dof, qval*flux_val);

        }//Dirichlet
        else if (type == BCType::NEUMANN) {

            auto gradient = boundary_func_->gradients(face.centroid());
            auto normal_comp = dot(gradient, face.normal_vector());
            uint_t var_dof = cell_dofs_[0].id;
            b_.add(var_dof, normal_comp);
        }
        else if(type == BCType::ZERO_NEUMANN){
            // nothing to do here.
        }
    }
}

template<int dim, typename Executor>
template<typename MatrixTp, typename VectorTp>
void
FVLaplaceAssemblyPolicyThreaded<dim, Executor>::AssembleTask<MatrixTp,VectorTp>::assemble_one_element(){

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

    if(volume_func_){
        row_entries[0] += volume_func_->value(elem_->centroid())*elem_->volume();
    }

    mat_.set_entry(row_dofs[0], row_dofs[0], row_entries[0]);
    for(uint_t idx=1; idx<row_dofs.size(); ++idx){
        mat_.set_entry(row_dofs[0], row_dofs[idx], row_entries[idx]);
    }

    real_t rhs_val = 0.0;
    if(rhs_func_ != nullptr){
        rhs_val = rhs_func_->value(elem_->centroid());
    }

    b_.add(cell_dofs_[0].id, rhs_val*elem_->volume());

    if(!boundary_faces.empty() && boundary_func_ != nullptr){
        apply_boundary_conditions(boundary_faces);
    }
}

}

}

#endif // FV_LAPLACE_ASSEMBLE_POLICY_THREADED_H
