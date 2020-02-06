#ifndef ELEMENT_H
#define ELEMENT_H

#include "kernel/discretization/mesh_entity.h"
#include "kernel/numerics/dof_object.h"
#include "kernel/numerics/dof.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/discretization/edge_face_selector.h"
#include "kernel/discretization/element_traits.h"

namespace kernel
{
namespace numerics
{

template<int dim> class Node;

/// \brief Wraps the notion of an element
template<int dim>
class Element: public MeshEntity
{

public:

    typedef Element<dim>* neighbor_ptr_t;
    typedef Element<dim>& neighbor_ref_t;
    typedef Node<dim>* node_ptr_t;
    typedef typename element_traits<Element<dim>>::edge_ptr_t edge_ptr_t;
    typedef typename element_traits<Element<dim>>::cedge_ptr_t cedge_ptr_t;
    typedef typename element_traits<Element<dim>>::edge_ref_t edge_ref_t;
    typedef typename element_traits<Element<dim>>::cedge_ref_t cedge_ref_t;
    typedef typename element_traits<Element<dim>>::face_ptr_t face_ptr_t;
    typedef typename element_traits<Element<dim>>::cface_ptr_t cface_ptr_t;
    typedef typename element_traits<Element<dim>>::face_ref_t face_ref_t;
    typedef typename element_traits<Element<dim>>::cface_ref_t cface_ref_t;

    /// \brief Destructor
    virtual ~Element();

    /// \brief How many vertices the element has
    virtual uint_t n_vertices()const=0;

    /// \brief How many nodes the element has
    virtual uint_t n_nodes()const=0;

    /// \brief Set the i-th node
    virtual void set_node(uint_t i, node_ptr_t node)=0;

    /// \brief Append a  node to the nodes list
    virtual void append_node(node_ptr_t node)=0;

    /// \brief Reserve space for nodes
    virtual void reserve_nodes(uint n)=0;

    /// \brief Returns the i-th node
    virtual node_ptr_t get_node(uint_t n)=0;

    /// \brief How many edges the element has
    virtual uint_t n_edges()const=0;

    /// \brief How many faces the element has
    virtual uint_t n_faces()const=0;

    /// \brief Returns the f-face
    virtual cface_ref_t get_face(uint_t f)const=0;

    /// \brief Returns the f-face
    virtual face_ref_t get_face(uint_t f)=0;

    /// \brief Set the i-th neighbor
    virtual void set_neighbor(uint n, neighbor_ptr_t neigh)=0;

    /// \brief Returns the number of neighbors
    virtual uint_t n_neighbors()const=0;

    /// \brief Reserve space for neighbors
    virtual void reserve_neighbors(uint n)=0;

    /// \brief Access the n-th neighbor
    virtual const neighbor_ref_t get_neighbor(uint_t n)const=0;

    /// \brief Access the n-th neighbor pointer
    virtual neighbor_ptr_t neighbor_ptr(uint_t n);

    /// \brief Access the n-th neighbor pointer
    virtual const neighbor_ptr_t neighbor_ptr(uint_t n)const;

    /// \brief Invalidate the dofs associated with
    /// the given variable
    void invalidate_dofs(const std::string_view name);

    /// \brief Invalidate the dofs associated with
    /// the given variable
    void insert_dof(DoF&& dof);

    /// \brief Return the dofs for the given variable
    void get_dofs(std::string_view name, std::vector<DoF>& dofs)const{}

protected:

    /// \brief Constructor
    Element()=default;

    /// \brief Constructor
    Element(uint_t id, uint_t pid);

    /// \brief The neighbors of the element
    std::vector<neighbor_ptr_t> neginbors_;

    /// \brief The nodes of the element
    std::vector<node_ptr_t> nodes_;


    /// \brief The object that handles
    /// the DoFs on the element
    DoFObject dofs_;
};

}

}

#endif // ELEMENT_H
