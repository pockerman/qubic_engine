#include "kernel/numerics/fv_interpolation_factory.h"

namespace kernel{
namespace numerics{


template<int dim>
std::shared_ptr<FVInterpolateBase<dim>>
FVInterpolationFactory<dim>::build(FVInterpolationType type){

}

}
}
