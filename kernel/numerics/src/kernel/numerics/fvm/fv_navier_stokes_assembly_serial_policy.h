#ifndef FV_NAVIER_STOKES_ASSEMBLY_SERIAL_POLICY_H
#define FV_NAVIER_STOKES_ASSEMBLY_SERIAL_POLICY_H

#include <string>
#include <map>
#include <array>

namespace kernel {
namespace numerics {

template<int dim> class Element;
template<int dim> class Mesh;
template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;
template<int dim> class NumericVectorFunctionBase;


namespace fvm {

/// forward declarations
template<int dim> class FVInterpolateBase;
template<int dim> class FVDoFManager;


template<int dim, typename DivGradAssemblyTp, typename PressureAssemblyTp>
class FVNavierStokesAssemblySerialPolicy
{
public:

    ///
    /// \brief div_grad_policy_t
    ///
    typedef DivGradAssemblyTp div_grad_policy_t;

    ///
    /// \brief pressure_policy_t
    ///
    typedef PressureAssemblyTp pressure_policy_t;

    ///
    ///
    ///
    FVNavierStokesAssemblySerialPolicy();

    ///
    /// \brief Constructor
    ///
    FVNavierStokesAssemblySerialPolicy(const std::array<std::string, dim>& velocity, const std::string& pressure);

    ///
    /// \brief Compute the fluxes over the cell last
    /// reinitialized
    ///
    void compute_fluxes();

    ///
    /// \brief Set the function that describes the boundary conditions
    ///
    void set_boundary_function(const std::string& com_name, const BoundaryFunctionBase<dim>& func);

    ///
    /// \brief Set the function that describes the rhs conditions
    ///
    void set_rhs_function(const std::string& com_name, const NumericScalarFunction<dim>& func);

    ///
    /// \brief Set the function that describes the volume terms
    ///
    void set_volume_term_function(const NumericScalarFunction<dim>& func);

    ///
    /// \brief Set the object that describes the dofs
    ///
    void set_dof_manager(const FVDoFManager<dim>& dof_manager);

    ///
    /// \brief Set the mesh pointer
    ///
    void set_mesh(const Mesh<dim>& mesh);

    ///
    /// \brief assemble
    ///
    void assemble(){}

private:

    ///
    /// \brief div_grad_policy_. The policy responsible for assembling
    /// the div-grad velocity terms
    ///
    div_grad_policy_t div_grad_policy_;

   ///
   /// \brief The policy for assembling the pressure term
   ///
   pressure_policy_t pressure_policy_;


};

template<int dim, typename DivGradAssemblyTp, typename PressureAssemblyTp>
FVNavierStokesAssemblySerialPolicy<dim, DivGradAssemblyTp, PressureAssemblyTp>::FVNavierStokesAssemblySerialPolicy()
    :
      div_grad_policy_(),
      pressure_policy_()
{}

template<int dim, typename DivGradAssemblyTp, typename PressureAssemblyTp>
FVNavierStokesAssemblySerialPolicy<dim,
                                   DivGradAssemblyTp,
                                   PressureAssemblyTp>::FVNavierStokesAssemblySerialPolicy(const std::array<std::string, dim>& velocity,
                                                                                           const std::string& pressure)
    :
    div_grad_policy_(velocity),
    pressure_policy_(pressure)
{}

template<int dim, typename DivGradAssemblyTp, typename PressureAssemblyTp>
void
FVNavierStokesAssemblySerialPolicy<dim, DivGradAssemblyTp, PressureAssemblyTp>::set_mesh(const Mesh<dim>& mesh){

    div_grad_policy_.set_mesh(mesh);
    pressure_policy_.set_mesh(mesh);
}

template<int dim, typename DivGradAssemblyTp, typename PressureAssemblyTp>
void
FVNavierStokesAssemblySerialPolicy<dim, DivGradAssemblyTp, PressureAssemblyTp>::set_dof_manager(const FVDoFManager<dim>& dof_manager){

    div_grad_policy_.set_dof_manager(dof_manager);
    pressure_policy_.set_dof_manager(dof_manager);
}

template<int dim, typename DivGradAssemblyTp, typename PressureAssemblyTp>
void
FVNavierStokesAssemblySerialPolicy<dim, DivGradAssemblyTp, PressureAssemblyTp>::compute_fluxes(){

    div_grad_policy_.compute_fluxes();
    pressure_policy_.compute_fluxes();
}

}

}

}

#endif // FV_NAVIER_STOKES_ASSEMBLY_SERIAL_POLICY_H
