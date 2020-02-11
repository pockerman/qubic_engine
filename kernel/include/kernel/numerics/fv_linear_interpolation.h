#ifndef FV_LINEAR_INTERPOLATION_H
#define FV_LINEAR_INTERPOLATION_H

#include "kernel/numerics/fv_interpolate_base.h"


namespace kernel{
namespace numerics{

template<int dim> class NumericVectorFunctionBase;

template<int dim>
class FVLinearInterpolate: public FVInterpolateBase<dim>
{

public:

    /// \brief Constructor
    FVLinearInterpolate();

    /// \brief Constructor
    FVLinearInterpolate(real_t gamma);

    /// \brief destructor
    virtual ~FVLinearInterpolate();

    /// \brief Compute the fluxes for the given element
    virtual void compute_fluxes(const Element<dim>& elem, std::vector<real_t>& values)const override;


protected:

    /// \brief The weight used for the interpolation
    /// the default is 0.5 that corresponds to central
    /// differencing
    real_t gamma_;

};

}
}

#endif // FV_LINEAR_INTERPOLATION_H
