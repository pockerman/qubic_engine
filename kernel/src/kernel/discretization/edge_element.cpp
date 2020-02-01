#include "kernel/discretization/edge_element.h"

namespace kernel
{

namespace numerics
{
EdgeElem<1>::EdgeElem(uint_t id, uint_t pid)
    :
      Element<1>(id, pid)
{}
}
}
