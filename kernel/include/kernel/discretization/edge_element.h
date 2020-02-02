#ifndef EDGE_ELEMENT_H
#define EDGE_ELEMENT_H

#include "kernel/discretization/element.h"

namespace kernel
{

namespace numerics
{

template<int dim> class EdgeElem;

template<>
class EdgeElem<1>: public Element<1>
{
public:

    typedef Element<1>::neighbor_ptr_t neighbor_ptr_t;
    typedef Element<1>::node_ptr_t node_ptr_t;

    /// \brief Constructor
    EdgeElem(uint_t id, uint_t proc_id=0);

    /// \brief How many nodes the element has
    virtual uint_t n_nodes()const {return 2;}

    /// \brief Set the i-th node
    virtual void set_node(uint_t i, node_ptr_t node) override final;

    /// \brief Reserve space for nodes
    virtual void reserve_nodes(uint n);

    /// \brief Returns the i-th node
    virtual node_ptr_t get_node(uint_t n) override final;

    /// \brief How many vertices the element has
    virtual uint_t n_vertices()const override final{return 2;}

    /// \brief How many edges the element has
    virtual uint_t n_edges()const override final{return 2;}

    /// \brief How many faces the element has
    virtual uint_t n_faces()const override final{return 2;}

    /// \brief Set the i-th neighbor
    virtual void set_neighbor(uint n, neighbor_ptr_t neigh)override final;

    /// \brief Reserve space for neighbors
    virtual void reserve_neighbors(uint n)override final;

    /// \brief Access the n-th neighbor
    virtual neighbor_ptr_t get_neighbor(uint_t n) override final;

};


}
}

#endif // EDGE_ELEMENT_H
