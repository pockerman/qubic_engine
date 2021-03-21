#include "kernel/numerics/boundary_function_base.h"

namespace kernel{
namespace numerics{

template<int dim>
BoundaryFunctionBase<dim>::BoundaryFunctionBase()
    :
      FunctionBase<real_t, GeomPoint<dim>>(),
      boundary_conditions_()
{}


template<int dim>
void
BoundaryFunctionBase<dim>::set_bc_type(uint_t bidx, BCType type){

    boundary_conditions_[bidx] = type;
}



template<int dim>
BCType
BoundaryFunctionBase<dim>::bc_type(uint_t n_boundary)const{
    auto itr = boundary_conditions_.find(n_boundary);

    if(itr == boundary_conditions_.end()){
        return BCType::INVALID_BC_TYPE;
    }

    return itr->second;
}

template<int dim>
std::vector<uint_t>
BoundaryFunctionBase<dim>::get_boundaries(BCType type)const{

    std::vector<uint_t> boundaries;

    if(boundary_conditions_.empty()){
        return boundaries;
    }

    boundaries.reserve(boundary_conditions_.size());

    auto itr = boundary_conditions_.begin();

    for(; itr != boundary_conditions_.end(); itr++){
        if(itr->second == type){
            boundaries.push_back(itr->first);
        }
    }

    return boundaries;
}

template class BoundaryFunctionBase<1>;
template class BoundaryFunctionBase<2>;
template class BoundaryFunctionBase<3>;

}
}
