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

    /// \brief Constructor
    EdgeElem(uint_t id, uint_t proc_id=0);

    /// \brief How many vertices the element has
    virtual uint_t n_vertices()const override final{return 2;}

    /// \brief How many edges the element has
    virtual uint_t n_edges()const override final{return 2;}

    /// \brief How many faces the element has
    virtual uint_t n_faces()const override final{return 2;}

};


}
}

#endif // EDGE_ELEMENT_H
