#include "kernel/discretization/element.h"

namespace kernel
{

namespace numerics
{

template<int dim>
Element<dim>::Element(uint_t id, uint_t pid)
    :
      MeshEntity(id, pid)
{}


template class Element<1>;
template class Element<2>;
template class Element<3>;

}

}
