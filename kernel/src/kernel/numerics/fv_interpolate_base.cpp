#include "kernel/numerics/fv_interpolate_base.h"

namespace kernel{
namespace numerics{

template<int dim>
FVInterpolateBase<dim>::FVInterpolateBase(FVInterpolationType type)
    :
      type_(type)
{}

template<int dim>
FVInterpolateBase<dim>::~FVInterpolateBase()
{}


template class FVInterpolateBase<1>;
template class FVInterpolateBase<2>;
template class FVInterpolateBase<3>;
}
}
