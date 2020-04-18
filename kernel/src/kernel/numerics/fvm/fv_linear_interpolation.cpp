#include "kernel/numerics/fvm/fv_linear_interpolation.h"

namespace kernel{
namespace numerics{


template<int dim>
FVLinearInterpolate<dim>::FVLinearInterpolate()
    :
      FVInterpolateBase<dim>(FVInterpolationType::LINEAR),
      gamma_(0.5)
{}


template<int dim>
FVLinearInterpolate<dim>::FVLinearInterpolate(real_t gamma)
    :
      FVInterpolateBase<dim>(FVInterpolationType::LINEAR),
      gamma_(gamma)
{}

template<int dim>
FVLinearInterpolate<dim>::~FVLinearInterpolate()
{}


template<int dim>
void
FVLinearInterpolate<dim>::compute_fluxes(const Element<dim>& elem, std::vector<real_t>& values)const{

    {
        std::vector<real_t> empty;
        values.swap(empty);
    }


}

template class FVLinearInterpolate<1>;
template class FVLinearInterpolate<2>;
template class FVLinearInterpolate<3>;

}
}
