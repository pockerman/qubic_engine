#include "kernel/discretization/node.h"
#include "kernel/base/kernel_consts.h"
namespace kernel
{

namespace numerics
{
template<int dim>
Node<dim>::Node()
    :
   id_(KernelConsts::invalid_size_type()),
   is_vertex_(false)
{}

template<int dim>
Node<dim>::Node(real_t coord, uint_t id)
    :
      id_(id),
      is_vertex_(false)
   {}

template<int dim>
Node<dim>::Node(const std::array<real_t, dim>& coords, uint_t id)
    :
      id_(id),
      is_vertex_(false)
   {}



// explicit instantiations
template class Node<1>;
template class Node<2>;
template class Node<3>;
}
}
