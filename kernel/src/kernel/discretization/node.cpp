#include "kernel/discretization/node.h"

namespace kernel
{

namespace numerics
{
template<int dim>
Node<dim>::Node()
    :
   GeomPoint<dim>(),
   DoFObject()
{}

template<int dim>
Node<dim>::Node(real_t coord)
    :
      GeomPoint<dim>(coord),
      DoFObject()
   {}

template<int dim>
Node<dim>::Node(const std::array<real_t, dim>& coords)
    :
      GeomPoint<dim>(coords),
      DoFObject()
   {}



// explicit instantiations
template class Node<1>;
template class Node<2>;
template class Node<3>;
}
}
