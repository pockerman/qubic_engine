#include "kernel/discretization/node.h"
#include "kernel/base/kernel_consts.h"
namespace kernel
{

namespace numerics
{
template<int dim>
Node<dim>::Node()
    :
   FaceElement<dim, 0> (KernelConsts::invalid_size_type()),
   //id_(KernelConsts::invalid_size_type()),
   is_vertex_(false)
{}

template<int dim>
Node<dim>::Node(real_t coord, uint_t id)
    :
      FaceElement<dim, 0> (GeomPoint<dim>(coord), id),
      //id_(id),
      is_vertex_(false)
   {}

template<int dim>
Node<dim>::Node(const std::array<real_t, dim>& coords, uint_t id)
    :
      FaceElement<dim, 0>(coords, id),
      is_vertex_(false)
   {}

template<int dim>
Node<dim>::Node(const GeomPoint<dim>& point, uint_t id, uint_t pid)
    :
      FaceElement<dim, 0>(point, id),
      is_vertex_(false)
{}



// explicit instantiations
template class Node<1>;
template class Node<2>;
template class Node<3>;
}
}
