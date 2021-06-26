#ifndef STOKES_ASSEMBLE_POLICY_H
#define STOKES_ASSEMBLE_POLICY_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/fvm/fv_vector_assemble_policy.h"

#include <array>
namespace kernel{
namespace numerics {
namespace fvm {

template<int dim>
class FVStokesAssemblyPolicy: public FVVectorAssemblePolicy<dim>
{
  public:

    ///
    /// \brief Constructor
    ///
    FVStokesAssemblyPolicy(const std::array<std::string, dim>& components);

    ///
    /// \brief Compute the fluxes over the cell last
    /// reinitialized
    ///
    virtual void compute_fluxes() override;

    ///
    /// \brief Reinitialize the policy
    ///
    virtual void reinit(const Element<dim>& element) override;

    ///
    /// \brief reinit
    ///
    virtual void reinit(const std::array<std::string, dim>& components) override;
};

}

}
}
#endif
#endif // STOKES_ASSEMBLE_POLICY_H
