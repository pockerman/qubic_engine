#include "kernel/numerics/fem/finite_element_base.h"

#ifdef USE_FEM

namespace kernel{
namespace numerics {
namespace fem {

template<int dim>
FiniteElementBase<dim>::FiniteElementBase(const FE_Type type)
    :
      fe_type_(type),
      element_ptr_(nullptr)
{}


template class FiniteElementBase<1>;
template class FiniteElementBase<2>;
template class FiniteElementBase<3>;
}
}
}

#endif
