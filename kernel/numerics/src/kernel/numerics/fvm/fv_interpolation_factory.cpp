#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/fvm/fv_interpolation_factory.h"
#include "kernel/numerics/fvm/fv_ud_interpolation.h"
#include "kernel/numerics/fvm/fv_linear_interpolation.h"

#include <exception>

namespace kernel{
namespace numerics{


template<int dim>
std::shared_ptr<FVInterpolateBase<dim>>
FVInterpolationFactory<dim>::build(FVInterpolationType type){

    std::shared_ptr<FVInterpolateBase<dim>> ptr;

    switch (type) {
    case FVInterpolationType::UD:
        ptr.reset(new FVUDInterpolate<dim>());
        break;
    case FVInterpolationType::LINEAR:
        ptr.reset(new FVLinearInterpolate<dim>());
        break;
    default:
        throw std::logic_error("Invalid FVInterpolationType type");
    }


    return ptr;
}

template class FVInterpolationFactory<1>;
template class FVInterpolationFactory<2>;
template class FVInterpolationFactory<3>;

}
}
#endif
