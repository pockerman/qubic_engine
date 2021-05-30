#ifndef FV_GAUSS_GRAD_H
#define FV_GAUSS_GRAD_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/fvm/fv_grad_base.h"

namespace kernel {

namespace numerics {

template<int dim>
class FVGaussGrad: public FVGradBase<dim>
{

public:

    /// \brief Constructor
    FVGaussGrad();

    /// \brief Compute the gradients for the given element
    virtual void compute_gradients(const Element<dim>& elem, std::vector<real_t>& values)const override;

    /// \brief Returns true if the a approximation
    /// uses some sort of correction
    virtual bool is_corrected()const override{return false;}

};

}
}

#endif
#endif // FV_GAUSS_GRAD_H
