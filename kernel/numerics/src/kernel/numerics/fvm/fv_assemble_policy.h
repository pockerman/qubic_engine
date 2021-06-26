#ifndef FV_ASSEMBLE_POLICY_H
#define FV_ASSEMBLE_POLICY_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/base/types.h"
#include "kernel/discretization/dof.h"
     
#include <vector>

namespace kernel {
namespace numerics{

/// forward declarations
template<int dim> class FVInterpolateBase;
template<int dim> class Element;
template<int dim> class Mesh;
template<int dim> class FVDoFManager;
template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;
template<int dim> class NumericVectorFunctionBase;

#ifdef USE_TRILINOS
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;
#endif

namespace fvm {


///
/// \brief Base class for deriving
/// assemble policies for FVM
///
template<int dim>
class FVAssemblyPolicy
{
public:

    ///
    /// \brief ~FVAssemblyPolicy
    ///
    virtual ~FVAssemblyPolicy()=default;

    ///
    /// \brief Compute the fluxes over the cell last
    /// reinitialized
    ///
    virtual void compute_fluxes()=0;

    ///
    /// \brief Reinitialize the policy
    ///
    virtual void reinit(const Element<dim>& element);

    ///
    /// \brief Reinitialize the policy
    ///
    virtual void reinit(const Element<dim>& element, const std::vector<real_t>& qvals);

#ifdef USE_TRILINOS

    ///
    /// \brief Assemble the data
    ///
    virtual void assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b )=0;

    ///
    /// \brief Apply the boundary conditions
    ///
    virtual void apply_boundary_conditions(const  std::vector<uint_t>& bfaces, TrilinosEpetraMatrix& mat,
                                           TrilinosEpetraVector& x, TrilinosEpetraVector& b )=0;

    ///
    /// \brief assemble one element contribution
    ///
    virtual void assemble_one_element(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b )=0;

#endif

    ///
    /// \brief Set the function that describes the boundary conditions
    ///
    void set_boundary_function(const BoundaryFunctionBase<dim>& func){boundary_func_ = &func;}

    ///
    /// \brief Set the function that describes the boundary conditions
    ///
    void set_rhs_function(const NumericScalarFunction<dim>& func){rhs_func_ = &func;}

    ///
    /// \brief Set the function that describes the boundary conditions
    ///
    void set_volume_term_function(const NumericScalarFunction<dim>& func){volume_func_ = &func;}

    ///
    /// \brief Set the object that describes the dofs
    ///
    void set_dof_manager(const FVDoFManager<dim>& dof_manager){dof_manager_ = &dof_manager;}

    ///
    /// \brief Set the mesh pointer
    ///
    void set_mesh(const Mesh<dim>& mesh){m_ptr_ = &mesh;}

protected:

    ///
    /// \brief Constructor
    ///
    FVAssemblyPolicy(const std::string& name);


    ///
    /// \brief name_ Name of the variable the assembly
    /// policy is working on
    ///
    std::string name_;

    ///
    /// \brief The element over which the policy is working
    ///
    const Element<dim>* elem_;

    ///
    /// \brief The values over the cell sides
    ///
    std::vector<real_t> qvals_;

    ///
    /// \brief The dofs of the neighbors of the cell we do work on
    ///
    std::vector<DoF> neigh_dofs_;

    ///
    /// \brief The dofs of the cell
    ///
    std::vector<DoF> cell_dofs_;

    ///
    /// \brief The cell fluxes
    ///
    std::vector<real_t> fluxes_;

    ///
    /// \brief The DoFManager that handles the dofs
    ///
    const FVDoFManager<dim>* dof_manager_;

    ///
    /// \brief The boundary function used
    ///
    const BoundaryFunctionBase<dim>* boundary_func_;

    ///
    /// \brief Pointer to the function object that describes the rhs
    ///
    const NumericScalarFunction<dim>* rhs_func_;

    ///
    /// \brief Pointer to the function object that describes the
    /// any volume terms to assemble
    ///
    const NumericScalarFunction<dim>* volume_func_;

    ///
    /// \brief The object responsible for calculating the velocity term
    ///
    const NumericVectorFunctionBase<dim>* velocity_func_;

    ///
    /// \brief The Mesh over which the policy is working
    ///
    const Mesh<dim>* m_ptr_;

    ///
    /// \brief initialize dofs
    ///
    void initialize_dofs_();

};

}
}
}
#endif
#endif // FV_ASSEMBLE_POLICY_H
