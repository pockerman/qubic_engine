#ifndef LINE_MESH_H
#define LINE_MESH_H

#include "kernel/base/types.h"
#include <vector>


namespace kernel
{

namespace numerics
{

template<int dim> class Node;
template<int dim> class EdgeElem;

/// \brief Models a LineMesh
class LineMesh
{
public:

    typedef Node<1> edge_t;
    typedef Node<1> face_t;
    typedef Node<1> node_t;
    typedef node_t* node_ptr_t;
    typedef EdgeElem<1> elem_t;
    typedef elem_t* elem_ptr_t;

    /// \brief Constructor
    LineMesh()=default;

    /// \brief How many elements
    uint_t n_elements()const{return elements_.size();}

    /// \brief How many nodes
    uint_t n_nodes()const{return nodes_.size();}

    /// \brief How many sides
    uint_t n_edges()const;

    /// \brief How many sides
    uint_t n_faces()const;

    /// \brief Create a new node
    /// and get back the pointer
    node_ptr_t add_node(real_t x);

    /// \brief Create a new element and get
    /// back the pointer
    elem_ptr_t add_element();

private:

    /// \brief List of nodes in the mesh
    std::vector<node_ptr_t> nodes_;

    /// \brief List of elements in the mesh
    std::vector<elem_ptr_t> elements_;

};

}
}

#endif // LINE_MESH_H
