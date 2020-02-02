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
    Node(real_t coord, uint_t id);

    /// \brief Constructor
    Node(const std::array<real_t, dim>& coords, uint_t id);

    /// \brief Returns the id of the node
    uint_t get_id()const{return id_;}

    /// \brief Set the id of the node
    void set_id(uint_t id){id_ = id;}

    /// \brief Returns true is the Node is a vertex
    bool is_vertex()const{return is_vertex_;}

    /// \brief Signal the Node that it is a vertex
    void make_vertex(){is_vertex_ = true;}

private:

    /// \brief the id of the node
    uint_t id_;

    /// \brief is the Node a vertex
    bool is_vertex_;

};

}

}

#endif // NODE_H
