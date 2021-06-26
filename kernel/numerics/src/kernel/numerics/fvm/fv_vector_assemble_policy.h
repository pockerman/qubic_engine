#ifndef FV_VECTOR_ASSEMBLE_POLICY_H
#define FV_VECTOR_ASSEMBLE_POLICY_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/fvm/fv_assemble_policy.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#include <array>
#include <string>
#include <map>
#include <memory>

namespace kernel {
namespace numerics {
namespace fvm {

///
/// \brief Base class for vector finite volume assembly
/// policies.
///
template<int dim>
class FVVectorAssemblePolicy
{
public:

    ///
    /// \brief destructor
    ///
    virtual ~FVVectorAssemblePolicy()=default;

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
    /// \brief reinit
    ///
    virtual void reinit(const std::array<std::string, dim>& components);

    ///
    /// \brief Reinitialize the policy
    ///
    virtual void reinit(const std::string& comp_name, const Element<dim>& element, const std::vector<real_t>& qvals)
    {{comp_assmble_policies_[comp_name]->reinit(element, qvals);}}

    ///
    /// \brief Set the function that describes the boundary conditions
    ///
    void set_boundary_function(const std::string& comp_name, const BoundaryFunctionBase<dim>& func)
    {comp_assmble_policies_[comp_name]->set_boundary_function(func);}

    ///
    /// \brief Set the function that describes the boundary conditions
    ///
    void set_rhs_function(const std::string& comp_name, const NumericScalarFunction<dim>& func)
    {{comp_assmble_policies_[comp_name]->set_rhs_function(func);}}

    ///
    /// \brief Set the function that describes the boundary conditions
    ///
    void set_volume_term_function(const std::string& comp_name, const NumericScalarFunction<dim>& func)
    {{comp_assmble_policies_[comp_name]->set_volume_term_function(func);}}

    ///
    /// \brief Set the object that describes the dofs
    ///
    void set_dof_manager(const std::string& comp_name, const FVDoFManager<dim>& dof_manager);

    ///
    /// \brief Set the mesh pointer
    ///
    void set_mesh(const Mesh<dim>& mesh);

protected:

    ///
    /// \brief FVVectorAssemblePolicy
    ///
    FVVectorAssemblePolicy(const std::array<std::string, dim>& components);


    ///
    /// \brief comp_assmble_policies_ Map for the component cpolicies of the vector
    ///
    std::map<std::string, std::shared_ptr<FVAssemblyPolicy<dim>> > comp_assmble_policies_;

    ///
    /// \brief get_components
    ///
    std::map<std::string, std::shared_ptr<FVAssemblyPolicy<dim>> >& get_components()
    {return comp_assmble_policies_;}

};


}
}
}
#endif
#endif // FV_VECTOR_ASSEMBLE_POLICY_H
