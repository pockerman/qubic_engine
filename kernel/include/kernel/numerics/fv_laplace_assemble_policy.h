#ifndef FV_LAPLACE_ASSEMBLE_POLICY_H
#define FV_LAPLACE_ASSEMBLE_POLICY_H

#include "kernel/base/types.h"


#include "boost/any.hpp"
#include <memory>
#include <vector>
#include <string>

namespace kernel {
namespace numerics {

/// forward declarations
template<int dim> class FVGradBase;
template<int dim> class Element;
template<int dim> class FVDoFManager;
template<int dim> class BoundaryFunctionBase;

class TrilinosEpetraMatrix;
class TrilinosEpetraVector;

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

    /// \brief Assemble the data
    void assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b )const;

    /// \brief Apply the boundary conditions
    void apply_boundary_conditions(const  std::vector<boost::any>& bfaces, TrilinosEpetraMatrix& mat,
                                   TrilinosEpetraVector& x, TrilinosEpetraVector& b )const;

    /// \brief Compute the fluxes over the cell last
    /// reinitialized
    void compute_fluxes();
private:

    /// \brief Pointer to the FV gradient approximation
    std::shared_ptr<FVGradBase<dim>> fv_grads_;

    /// \brief The element over which the policy is working
    const Element<dim>* elem_;

    /// \brief The values over the cell sides
    std::vector<real_t> qvals_;

    /// \brief The dofs of the neighbors of the cell we do work on
    std::vector<uint_t> neigh_dofs_;

    /// \brief The dofs of the cell
    std::vector<uint_t> cell_dofs_;

    /// \brief The cell fluxes
    std::vector<real_t> fluxes_;

    /// \brief The DoFManager that handles the dofs
    const FVDoFManager<dim>* dof_manager_;

    /// \brief The boundary function used
    const BoundaryFunctionBase<dim>* boundary_func_;

    /// \brief initialize dofs
    void initialize_dofs_();
};


}

}

#endif // FV_LAPLACE_ASSEMBLE_POLICY_H
