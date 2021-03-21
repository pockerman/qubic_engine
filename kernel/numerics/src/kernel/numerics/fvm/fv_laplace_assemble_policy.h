#ifndef FV_LAPLACE_ASSEMBLE_POLICY_H
#define FV_LAPLACE_ASSEMBLE_POLICY_H

#include "kernel/base/types.h"
#include "kernel/base/config.h"
#include "kernel/discretization/dof.h"

#include <vector>
#include <string>

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
template<int dim>
class FVLaplaceAssemblyPolicy
{
public:

    /// \brief Constructor
    FVLaplaceAssemblyPolicy();

    /// \brief Reinitialize the policy
    void reinit(const Element<dim>& element);

    /// \brief Reinitialize the policy
    void reinit(const Element<dim>& element, std::vector<real_t>&& qvals);

#ifdef USE_TRILINOS

    /// \brief Assemble the data
    void assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b );

    /// \brief Apply the boundary conditions
    void apply_boundary_conditions(const  std::vector<uint_t>& bfaces, TrilinosEpetraMatrix& mat,
                                   TrilinosEpetraVector& x, TrilinosEpetraVector& b );

    /// \brief assemble one element contribution
    void assemble_one_element(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b );

#endif

    /// \brief Compute the fluxes over the cell last
    /// reinitialized
    void compute_fluxes();

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

    /// \brief Access the mesh
    const Mesh<dim>& get_mesh()const{return *m_ptr_;}

    /// \brief Access the DoFs associated with the cell
    const std::vector<DoF>& get_element_dofs()const{return cell_dofs_;}

    /// \brief Access the DoFs associated with the neighbors
    /// of the cell
    const std::vector<DoF>& get_neighbor_dofs()const{return neigh_dofs_;}

    /// \brief Returns the computed elementvolume
    real_t get_element_volume()const{return elem_volume_;}

    /// \brief Build the  gradient scheme
    template<typename Factory>
    void build_gradient(const Factory& factory);

    /// \brief initialize dofs
    void initialize_dofs();

private:

    /// \brief Pointer to the FV gradient approximation
    std::shared_ptr<FVGradBase<dim>> fv_grads_;

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

    /// \brief the volume of the element
    real_t elem_volume_;

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


};

template<int dim>
template<typename Factory>
void
FVLaplaceAssemblyPolicy<dim>::build_gradient(const Factory& factory){

    fv_grads_ = factory();
}

}

}

#endif // FV_LAPLACE_ASSEMBLE_POLICY_H
