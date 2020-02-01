#include "kernel/discretization/element.h"

namespace kernel
{

namespace numerics
{

namespace detail
{
element_base::element_base(uint_t id, uint_t pid)
    :
      DoFObject(),
      id_(id),
      pid_(pid)
{}

element_base::~element_base()
{}



}//detail

template<int dim>
Element<dim>::Element(uint_t id, uint_t pid)
    :
      detail::element_base(id, pid)
{}


template class Element<1>;
template class Element<2>;
template class Element<3>;

}

}
