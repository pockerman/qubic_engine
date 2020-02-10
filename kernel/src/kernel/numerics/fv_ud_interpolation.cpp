#include "kernel/numerics/fv_ud_interpolation.h"
#include <exception>
namespace kernel{
namespace numerics{


template<int dim>
FVUDInterpolate<dim>::FVUDInterpolate()
    :
    FVInterpolateBase<dim>(FVInterpolationType::UD),
    velocity_(nullptr)
{}

template<int dim>
FVUDInterpolate<dim>::FVUDInterpolate(const NumericVectorFunctionBase<dim>& velocity)
    :
    FVInterpolateBase<dim>(FVInterpolationType::UD),
    velocity_(&velocity)
{}

template<int dim>
FVUDInterpolate<dim>::~FVUDInterpolate()
{}

template<int dim>
void
FVUDInterpolate<dim>::compute_fluxes(const Element<dim>& elem, std::vector<real_t>& values)const{

    if(!velocity_){
        throw std::logic_error("Velocity pointer is NULL");
    }
}

template class FVUDInterpolate<1>;
template class FVUDInterpolate<2>;
template class FVUDInterpolate<3>;
}
}
