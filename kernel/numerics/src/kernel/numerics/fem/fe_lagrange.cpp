#include "kernel/numerics/fem/fe_lagrange.h"

#ifdef USE_FEM

namespace kernel{
namespace numerics {
namespace fem {

template<int dim>
FELagrange<dim>::FELagrange(const FE_Type type)
    :
      FiniteElementBase<dim> (type)
{}


}
}
}

#endif
