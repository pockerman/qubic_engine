#include "kernel/numerics/scalar_dirichlet_bc_function.h"
#include "kernel/numerics/boundary_conditions_type.h"
namespace kernel{
namespace numerics{

template<int dim>
ScalarDirichletBCFunc<dim>::ScalarDirichletBCFunc(real_t val)
    :
      BoundaryFunctionBase<dim>(),
      value_(val)
{}

template<int dim>
ScalarDirichletBCFunc<dim>::ScalarDirichletBCFunc(real_t val, uint_t n_boundaries)
    :
ScalarDirichletBCFunc<dim>(val)
{
    for(uint_t b=0; b<n_boundaries; ++b){
        this->set_bc_type(b , BCType::DIRICHLET);
    }
}

template class ScalarDirichletBCFunc<1>;
template class ScalarDirichletBCFunc<2>;
template class ScalarDirichletBCFunc<3>;

}
}
