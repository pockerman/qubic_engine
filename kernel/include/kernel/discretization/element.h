#ifndef ELEMENT_H
#define ELEMENT_H

#include "kernel/numerics/dof_object.h"
#include "kernel/base/kernel_consts.h"

namespace kernel
{
namespace numerics
{

namespace detail
{

class element_base: public DoFObject
{
public:

    /// \brief Destructor
    virtual ~element_base();

    /// \brief Returns the id of the element
    uint_t get_id()const{return id_;}

    /// \brief Set the id of the element
    void set_id(uint_t id){id_ = id;}

    /// \brief Returns the id of the element
    uint_t get_pid()const{return pid_;}

    /// \brief Set the id of the element
    void set_pid(uint_t id){pid_ = id;}


protected:

    /// \brief Constructor
    element_base(uint_t id=KernelConsts::invalid_size_type(), uint_t pid=KernelConsts::invalid_size_type());

    /// \brief The id of the element
    uint_t id_;

    /// \brief The processor id the element belongs
    uint_t pid_;

};
}



/// \brief Wraps the notion of an element
template<int dim>
class Element: public detail::element_base
{

public:

    /// \brief How many vertices the element has
    virtual uint_t n_vertices()const=0;

    /// \brief How many edges the element has
    virtual uint_t n_edges()const=0;

    /// \brief How many faces the element has
    virtual uint_t n_faces()const=0;


protected:

    /// \brief Constructor
    Element()=default;

    /// \brief Constructor
    Element(uint_t id, uint_t pid);

};

}

}

#endif // ELEMENT_H
