#ifndef FV_UD_INTERPOLATION_H
#define FV_UD_INTERPOLATION_H

#include "kernel/numerics/fv_interpolate_base.h"

namespace kernel{
namespace numerics{

template<int dim> class NumericVectorFunctionBase;

template<int dim>
class FVUDInterpolate: public FVInterpolateBase<dim>
{

public:

    /// \brief Constructor
    FVUDInterpolate();

    /// \brief Constructor
    FVUDInterpolate(const NumericVectorFunctionBase<dim>& velocity);

    /// \brief destructor
    virtual ~FVUDInterpolate();

    /// \brief Compute the fluxes for the given element
    virtual void compute_fluxes(const Element<dim>& elem, std::vector<real_t>& values)const override;


protected:


    /// \brief Pointer to the object describing the boundary
    /// conditions
    const NumericVectorFunctionBase<dim>* velocity_;

};

}
}

#endif // FV_UD_INTERPOLATION_H
