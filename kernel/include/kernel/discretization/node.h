#ifndef NODE_H
#define NODE_H

#include "kernel/base/types.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/numerics/dof_object.h"

#include <array>

namespace kernel
{

namespace numerics
{

/// \brief Wraps the notion of a node. A node
/// is simply a point in dim-space that can hold
/// dofs
template<int dim>
class Node: public GeomPoint<dim>,
            DoFObject
{

public:

    /// \brief Constructor
    Node();

    /// \brief Constructor
    Node(real_t coord);

    /// \brief Constructor
    Node(const std::array<real_t, dim>& coords);

};

}

}

#endif // NODE_H
