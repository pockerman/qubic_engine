#include "kernel/numerics/fv_interpolation_factory.h"
#include "kernel/numerics/fv_ud_interpolation.h"
#include "kernel/numerics/fv_linear_interpolation.h"

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
    case FVInterpolationType::LINEAR:
        ptr.reset(new FVLinearInterpolate<dim>());
    default:
        throw std::logic_error("Invalid FVInterpolationType type");
    }


    return ptr;
}

}
}
