#ifndef MESH_H
#define MESH_H

#include "kernel/base/types.h"
#include "kernel/discretization/mesh_topology.h"

namespace kernel
{
namespace numerics
{


/// \brief A class that represents a mesh.
template<int spacedim>
class Mesh<spacedim>
{


  public:

    typedef typename MeshTopology<spacedim>::edge_ptr edge_ptr;
    typedef typename MeshTopology<spacedim>::face_ptr face_ptr;
    typedef typename MeshTopology<spacedim>::const_edge_ptr const_edge_ptr;
    typedef typename MeshTopology<spacedim>::const_face_ptr const_face_ptr;

    const static int dimension = spacedim;

    /// \brief Constructor. Creates an empty Mesh
    Mesh();

    /// \brief dtor
    virtual ~Mesh();


   void reserve_n_nodes(size_type n){topology_.reserve_n_nodes(n);}



   void reserve_n_elements(size_type n){topology_.reserve_n_elements(n);}



   void reserve_n_edges(size_type n){topology_.reserve_n_edges(n);}


   void reserve_n_faces(size_type n){topology_.reserve_n_faces(n);}

   /**
     *\detailed create a vertex by passing in the coordinates
     *of the vertex. This will create a Node so optionally we can
     *pass in the global id and the processor id of the created node
     */
   Node<spacedim>* create_vertex(const GeomPoint<spacedim>& point,
                                 size_type global_id=LibSimPP::invalid_uint(),
                                 size_type pid = 0);

   /**
     *\detailed create a Node
     */
   Node<spacedim>* create_node(const GeomPoint<spacedim>& point,
                               size_type global_id=LibSimPP::invalid_uint(),
                               size_type pid = 0);


   /**
     *\detailed create an Element and get back the pointer
     */
   Element<spacedim>* create_element(ElementType::sub_type t,
                                     size_type global_id=LibSimPP::invalid_uint(),
                                     size_type pid = 0);


   /**
     *\detailed create an edge and get back the pointer
     */
   edge_ptr  create_edge(ElementType::sub_type t,
                         size_type global_id=LibSimPP::invalid_uint(),
                         size_type pid = 0);


  /**
     *\detailed create a face and get back the pointer
     */
  face_ptr create_face(ElementType::sub_type t,
                       size_type global_id=LibSimPP::invalid_uint(),
                       size_type pid=0);


  /**
    * \detailed set the number of boundaries for the mesh
    */
  void set_n_boundaries(size_type nb){n_boundaries_ = nb;}


 @}
/**
 * @name Accessors
 */
 @{


 /**
   *\detailed read/write acces to the n-th node
   */
   Node<spacedim>* node(size_type n){return topology_.node(n);}


 /**
   *\detailed read acces to the n-th node
   */

  const Node<spacedim>* node(size_type n)const{return topology_.node(n);}


  /**
    *\detailed read/write access to the n-th element
    */
  Element<spacedim>* element(size_type e){return topology_.element(e);}


  /**
    *\detailed read access to the n-th element
    */
  const Element<spacedim>* element(size_type e)const{return topology_.element(e);}


   /**
     *\detailed how many vertices the mesh has
     */
   size_type n_vertices()const;


   /**
     *\detailed how many nodes the mesh has
     */
   size_type n_nodes()const{return topology_.n_nodes();}


   /**
     *\detailed how many elements the mesh has
     */
   size_type n_elements()const{return topology_.n_elements();}


   /**
     *\detailed how many edges the mesh has
     */
   size_type n_edges()const; //{return topology_.n_edges();}


   /**
     *\detailed how many faces the mesh has
     */
   size_type n_faces()const; //{return topology_.n_faces();}


   /**
     * get the active faces of the mesh
     */
   size_type n_active_faces()const;

   /**
     *\detailed return read/write access to the topology of the mesh
     */
   MeshTopology<spacedim>* topology(){return &topology_;}


   /**
     *\detailed read access to the topology of the mesh
     */
   const MeshTopology<spacedim>* topology()const{return &topology_;}


   /**
     *\detailed get the pointers to the faces described by the given MeshConnectivity
     */
   void faces(const MeshConnectivity& faces_idx,std::vector<const_face_ptr>& faces_ptr)const
   {topology_.faces(faces_idx,faces_ptr);}


   /**
     *\detailed get the number of boundaries of the mesh
     */
  size_type n_boundaries()const{return n_boundaries_;}


 /**
   * \detailed non-const iterator for the nodes
   * of the mesh
   */
 template<typename PREDICATE>
 struct node_iterator;

 /**
   *\detailed const iterator for the nodes of the mesh
   */
 template<typename PREDICATE>
 struct const_node_iterator;

 //start-end over all nodes of the mesh
 node_iterator<NotNull> nodes_begin();
 node_iterator<NotNull> nodes_end();

 const_node_iterator<NotNull> nodes_begin()const;
 const_node_iterator<NotNull> nodes_end()const;


 //start-end over the vertices of the mesh
 node_iterator<VertexNode> nodes_vertices_begin();
 node_iterator<VertexNode> nodes_vertices_end();

 const_node_iterator<VertexNode> nodes_vertices_begin()const;
 const_node_iterator<VertexNode> nodes_vertices_end()const;

 /**
   *\detailed the object that represents the iterator for the nodes
   */
 typedef typename std::vector<Node<spacedim>* >::iterator node_iterator_impl;
 typedef typename std::vector<Node<spacedim>* >::const_iterator const_node_iterator_impl;


 /**
   *\detailed non-const iterator for the elements of the mesh
   */
 template<typename PREDICATE>
 struct element_iterator;


 /**
   *\detailed const iterator for the elements of the mesh
   */
  template<typename PREDICATE>
  struct const_element_iterator;


  //start-end over all elements of the mesh
  element_iterator<NotNull> elements_begin();
  element_iterator<NotNull> elements_end();

  const_element_iterator<NotNull> elements_begin()const;
  const_element_iterator<NotNull> elements_end()const;

  //start-end over all active elements of the mesh
  element_iterator<Active> active_elements_begin();
  element_iterator<Active> active_elements_end();

  const_element_iterator<Active> active_elements_begin()const;
  const_element_iterator<Active> active_elements_end()const;


  /**
   *\detailed the object that represents the iterator for the elements
   */
 typedef typename std::vector<Element<spacedim>* >::iterator element_iterator_impl;
 typedef typename std::vector<Element<spacedim>* >::const_iterator const_element_iterator_impl;


 /**
   *\detailed non-const iterator for the edges of the mesh
   */
 template<typename PREDICATE>
 struct edge_iterator;


 /**
   *\detailed const iterator for the elements of the mesh
   */
  template<typename PREDICATE>
  struct const_edge_iterator;


  //start-end over all elements of the mesh
  edge_iterator<NotNull> edges_begin();
  edge_iterator<NotNull> edges_end();

  const_edge_iterator<NotNull> edges_begin()const;
  const_edge_iterator<NotNull> edges_end()const;

  //start-end over all active elements of the mesh
  edge_iterator<Active> active_edges_begin();
  edge_iterator<Active> active_edges_end();

  const_edge_iterator<Active> active_edges_begin()const;
  const_edge_iterator<Active> active_edges_end()const;


  /**
   *\detailed the object that represents the iterator for the elements
   */
 typedef typename std::vector<edge_ptr >::iterator edge_iterator_impl;
 typedef typename std::vector<edge_ptr >::const_iterator const_edge_iterator_impl;


 /**
   *\detailed non-const iterator for the edges of the mesh
   */
 template<typename PREDICATE>
 struct face_iterator;


 /**
   *\detailed const iterator for the elements of the mesh
   */
  template<typename PREDICATE>
  struct const_face_iterator;



  //start-end over all elements of the mesh
  face_iterator<NotNull> faces_begin();
  face_iterator<NotNull> faces_end();

  const_face_iterator<NotNull> faces_begin()const;
  const_face_iterator<NotNull> faces_end()const;

  //start-end over all active elements of the mesh
  face_iterator<Active> active_faces_begin();
  face_iterator<Active> active_faces_end();

  const_face_iterator<Active> active_faces_begin()const;
  const_face_iterator<Active> active_faces_end()const;



  //start-end over all elements of the mesh
  face_iterator<ActiveBoundaryObject> active_boundary_faces_begin();
  face_iterator<ActiveBoundaryObject> active_boundary_faces_end();

  const_face_iterator<ActiveBoundaryObject> active_boundary_faces_begin()const;
  const_face_iterator<ActiveBoundaryObject> active_boundary_faces_end()const;


  /**
   *\detailed the object that represents the iterator for the elements
   */
 typedef typename std::vector<face_ptr>::iterator face_iterator_impl;
 typedef typename std::vector<face_ptr>::const_iterator const_face_iterator_impl;



  private:

 /**
   *\detailed the number of boundaries of the mesh
   */
 size_type n_boundaries_;


  /**
    *\detailed describes how the various mesh
    * entities are connected
    */
  MeshTopology<spacedim> topology_;


};


template<int spacedim>
inline
size_type
Mesh<spacedim,serial_mode>::n_edges()const{
 return topology_.n_edges();
}

template<int spacedim>
inline
size_type
Mesh<spacedim,serial_mode>::n_faces()const{
return topology_.n_faces();
}

#ifdef BIBLSIM_DIM_1

template<>
inline
size_type
Mesh<1,serial_mode>::n_edges()const{
 return n_vertices();
}

template<>
inline
size_type
Mesh<1,serial_mode>::n_faces()const{
return n_edges();
}

#endif


//Node Iterator

  template<int dim>
  template<typename PREDICATE>
  struct
  Mesh<dim,serial_mode>::node_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                        typename Mesh<dim,serial_mode>::node_iterator_impl>
  {


     typedef typename Mesh<dim,serial_mode>::node_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;

     node_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}

     node_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

  };


  template<int dim>
  typename Mesh<dim,serial_mode>::template node_iterator<NotNull>
  Mesh<dim,serial_mode>::nodes_begin()
  {
    NotNull  p;
    return node_iterator<NotNull>(p,topology_.nodes_begin(),topology_.nodes_end());
  }

  template<int dim>
  typename Mesh<dim,serial_mode>::template node_iterator<NotNull>
  Mesh<dim,serial_mode>::nodes_end()
  {
    NotNull p;
    return node_iterator<NotNull>(p,topology_.nodes_end(),topology_.nodes_end());
  }

  template<int dim>
  typename Mesh<dim,serial_mode>::template node_iterator<VertexNode>
  Mesh<dim,serial_mode>::nodes_vertices_begin()
  {
   VertexNode p;
   return node_iterator<VertexNode>(p,topology_.nodes_begin(),topology_.nodes_end());
  }

  template<int dim>
  typename Mesh<dim,serial_mode>::template node_iterator<VertexNode>
  Mesh<dim,serial_mode>::nodes_vertices_end()
  {
    VertexNode p;
    return node_iterator<VertexNode>(p,topology_.nodes_end(),topology_.nodes_end());
  }

  template<int dim>
  template<typename PREDICATE>
  struct
  Mesh<dim,serial_mode>::const_node_iterator: public
               biblsimpp_filter_iterator<PREDICATE,
                                        typename Mesh<dim,serial_mode>::const_node_iterator_impl>
  {

     typedef typename Mesh<dim,serial_mode>::const_node_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;

     const_node_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}


    //template<typename PREDICATE>
    const_node_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

  };


  template<int dim>
  typename Mesh<dim,serial_mode>::template const_node_iterator<NotNull>
  Mesh<dim,serial_mode>::nodes_begin()const
  {
    NotNull  p;
    return const_node_iterator<NotNull>(p,topology_.nodes_begin(),topology_.nodes_end());
  }

  template<int dim>
  typename Mesh<dim,serial_mode>::template const_node_iterator<NotNull>
  Mesh<dim,serial_mode>::nodes_end()const
  {
   NotNull  p;
   return const_node_iterator<NotNull>(p,topology_.nodes_end(),topology_.nodes_end());
  }

  template<int dim>
  typename Mesh<dim,serial_mode>::template const_node_iterator<VertexNode>
  Mesh<dim,serial_mode>::nodes_vertices_begin()const
  {
   VertexNode p;
   return const_node_iterator<VertexNode>(p,topology_.nodes_begin(),topology_.nodes_end());
  }

  template<int dim>
  typename Mesh<dim,serial_mode>::template const_node_iterator<VertexNode>
  Mesh<dim,serial_mode>::nodes_vertices_end()const
  {
    VertexNode p;
    return const_node_iterator<VertexNode>(p,topology_.nodes_end(),topology_.nodes_end());
  }


//Element iterator

  template<int dim>
  template<typename PREDICATE>
  struct
  Mesh<dim,serial_mode>::element_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                           typename Mesh<dim,serial_mode>::element_iterator_impl>
  {

     typedef typename Mesh<dim,serial_mode>::element_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;


     element_iterator()
     :
     biblsimpp_filter_iterator<PREDICATE,iterator_impl>()
     {}

    element_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end)
    {}

  };


template<int dim>
typename Mesh<dim,serial_mode>::template element_iterator<NotNull >
Mesh<dim,serial_mode>::elements_begin()
{
  NotNull  p;
  return element_iterator<NotNull>(p,topology_.elements_begin(),topology_.elements_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template element_iterator<NotNull>
Mesh<dim,serial_mode>::elements_end()
{
  NotNull  p;
  return element_iterator<NotNull>(p,topology_.elements_end(),topology_.elements_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template element_iterator<Active>
Mesh<spacedim,serial_mode>::active_elements_begin()
{

Active p;
return element_iterator<Active>(p,topology_.elements_begin(),topology_.elements_end());

}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template element_iterator<Active>
Mesh<spacedim,serial_mode>::active_elements_end()
{

Active p;
return element_iterator<Active>(p,topology_.elements_end(),topology_.elements_end());

}




  template<int dim>
  template<typename PREDICATE>
  struct
  Mesh<dim,serial_mode>::const_element_iterator: public
               biblsimpp_filter_iterator<PREDICATE,
                                        typename Mesh<dim,serial_mode>::const_element_iterator_impl>
  {


     typedef typename Mesh<dim,serial_mode>::const_element_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;


     const_element_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}


    //template<typename PREDICATE>
    const_element_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

  };


template<int dim>
typename Mesh<dim,serial_mode>::template const_element_iterator<NotNull>
Mesh<dim,serial_mode>::elements_begin()const
{
  NotNull  p;
  return const_element_iterator<NotNull>(p,topology_.elements_begin(),topology_.elements_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_element_iterator<NotNull>
Mesh<dim,serial_mode>::elements_end()const
{
  NotNull  p;
  return const_element_iterator<NotNull>(p,topology_.elements_end(),topology_.elements_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_element_iterator<Active>
Mesh<spacedim,serial_mode>::active_elements_begin()const
{

Active p;
return const_element_iterator<Active>(p,topology_.elements_begin(),topology_.elements_end());

}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_element_iterator<Active>
Mesh<spacedim,serial_mode>::active_elements_end()const
{

Active p;
return const_element_iterator<Active>(p,topology_.elements_end(),topology_.elements_end());

}

//Edge iterator

  template<int dim>
  template<typename PREDICATE>
  struct
  Mesh<dim,serial_mode>::edge_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                        typename Mesh<dim,serial_mode>::edge_iterator_impl>
  {

     typedef typename Mesh<dim,serial_mode>::edge_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;


    edge_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}

    edge_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

  };

template<int dim>
typename Mesh<dim,serial_mode>::template edge_iterator<NotNull>
Mesh<dim,serial_mode>::edges_begin()
{
  NotNull  p;
  return edge_iterator<NotNull>(p,topology_.edges_begin(),topology_.edges_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template edge_iterator<NotNull >
Mesh<dim,serial_mode>::edges_end()
{
  NotNull p;
  return edge_iterator<NotNull >(p,topology_.edges_end(),topology_.edges_end());
}

//specializations for 1D edges
//==============================================================================================================
  template<>
  template<>
  struct
  Mesh<1,serial_mode>::edge_iterator<NotNull>: public biblsimpp_filter_iterator<VertexNotNull,
                                                                       Mesh<1,serial_mode>::edge_iterator_impl>
  {

     typedef typename Mesh<1,serial_mode>::edge_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::difference_type difference_type;


    edge_iterator()
    :
    biblsimpp_filter_iterator<VertexNotNull,iterator_impl>()
    {}

    edge_iterator(VertexNotNull p,iterator_impl begin ,iterator_impl end)
                          :
                biblsimpp_filter_iterator<VertexNotNull,iterator_impl>(p,begin,end){}

  };

  template<>
  template<>
  struct
  Mesh<1,serial_mode>::edge_iterator<Active>: public biblsimpp_filter_iterator<VertexActive,
                                                                       Mesh<1,serial_mode>::edge_iterator_impl>
  {

     typedef typename Mesh<1,serial_mode>::edge_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::difference_type difference_type;


    edge_iterator()
    :
    biblsimpp_filter_iterator<VertexActive,iterator_impl>()
    {}

    edge_iterator(VertexActive p,iterator_impl begin ,iterator_impl end)
                          :
                biblsimpp_filter_iterator<VertexActive,iterator_impl>(p,begin,end){}

  };

template<>
inline
typename Mesh<1,serial_mode>::template edge_iterator<NotNull>
Mesh<1,serial_mode>::edges_begin()
{
  VertexNotNull  p;
  return edge_iterator<NotNull>(p,topology_.edges_begin(),topology_.edges_end());
}

template<>
inline
typename Mesh<1,serial_mode>::template edge_iterator<NotNull>
Mesh<1,serial_mode>::edges_end()
{
  VertexNotNull p;
  return edge_iterator<NotNull >(p,topology_.edges_end(),topology_.edges_end());
}

template<>
inline
typename Mesh<1,serial_mode>::template edge_iterator<Active>
Mesh<1,serial_mode>::active_edges_begin()
{

VertexActive p;
return edge_iterator<Active>(p,topology_.edges_begin(),topology_.edges_end());

}

template<>
inline
typename Mesh<1,serial_mode>::template edge_iterator<Active>
Mesh<1,serial_mode>::active_edges_end()
{

VertexActive p;
return edge_iterator<Active>(p,topology_.edges_end(),topology_.edges_end());

}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template edge_iterator<Active>
Mesh<spacedim,serial_mode>::active_edges_begin()
{

Active p;
return edge_iterator<Active>(p,topology_.edges_begin(),topology_.edges_end());

}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template edge_iterator<Active>
Mesh<spacedim,serial_mode>::active_edges_end()
{

Active p;
return edge_iterator<Active >(p,topology_.edges_end(),topology_.edges_end());

}


template<int dim>
  template<typename PREDICATE>
  struct
  Mesh<dim,serial_mode>::const_edge_iterator: public
               biblsimpp_filter_iterator<PREDICATE,
                                        typename Mesh<dim,serial_mode>::const_edge_iterator_impl>
  {
     typedef typename Mesh<dim,serial_mode>::const_edge_iterator_impl iterator_impl;
      typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;

     const_edge_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}


    //template<typename PREDICATE>
    const_edge_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

  };


template<int dim>
typename Mesh<dim,serial_mode>::template const_edge_iterator<NotNull >
Mesh<dim,serial_mode>::edges_begin()const
{
  NotNull p;
  return const_edge_iterator<NotNull >(p,topology_.edges_begin(),topology_.edges_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_edge_iterator<NotNull>
Mesh<dim,serial_mode>::edges_end()const
{
  NotNull  p;
  return const_edge_iterator<NotNull >(p,topology_.edges_end(),topology_.edges_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_edge_iterator<Active>
Mesh<spacedim,serial_mode>::active_edges_begin()const
{

Active p;
return const_edge_iterator<Active>(p,topology_.edges_begin(),topology_.edges_end());

}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_edge_iterator<Active>
Mesh<spacedim,serial_mode>::active_edges_end()const
{

Active p;
return const_edge_iterator<Active>(p,topology_.edges_end(),topology_.edges_end());

}


//Face iterator

  template<int dim>
  template<typename PREDICATE>
  struct
  Mesh<dim,serial_mode>::face_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                        typename Mesh<dim,serial_mode>::face_iterator_impl>
  {

     typedef typename Mesh<dim,serial_mode>::face_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;


    face_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}

    face_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

  };

template<int dim>
typename Mesh<dim,serial_mode>::template face_iterator<NotNull >
Mesh<dim,serial_mode>::faces_begin()
{
  NotNull  p;
  return face_iterator<NotNull>(p,topology_.faces_begin(),topology_.faces_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template face_iterator<NotNull>
Mesh<dim,serial_mode>::faces_end()
{
  NotNull  p;
  return face_iterator<NotNull>(p,topology_.faces_end(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<Active>
Mesh<spacedim,serial_mode>::active_faces_begin()
{

Active p;
return face_iterator<Active>(p,topology_.faces_begin(),topology_.faces_end());

}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<Active>
Mesh<spacedim,serial_mode>::active_faces_end()
{

Active p;
return face_iterator<Active >(p,topology_.faces_end(),topology_.faces_end());

}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<ActiveBoundaryObject>
Mesh<spacedim,serial_mode>::active_boundary_faces_begin()
{

  ActiveBoundaryObject p;
  return face_iterator<ActiveBoundaryObject >(p,topology_.faces_begin(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<ActiveBoundaryObject>
Mesh<spacedim,serial_mode>::active_boundary_faces_end()
{

  ActiveBoundaryObject p;
  return face_iterator<ActiveBoundaryObject>(p,topology_.faces_end(),topology_.faces_end());
}

//specialization of faces for 1D
//=====================================================================================

  template<>
  template<>
  struct
  Mesh<1,serial_mode>::face_iterator<NotNull>: public biblsimpp_filter_iterator<VertexNotNull,
                                                                                Mesh<1,serial_mode>::face_iterator_impl>
  {

     typedef typename Mesh<1,serial_mode>::face_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<VertexNotNull,iterator_impl>::difference_type difference_type;


    face_iterator()
    :
    biblsimpp_filter_iterator<VertexNotNull,iterator_impl>(){}

    face_iterator(VertexNotNull p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<VertexNotNull,iterator_impl>(p,begin,end){}

  };

template<>
inline
Mesh<1,serial_mode>::template face_iterator<NotNull>
Mesh<1,serial_mode>::faces_begin()
{
  VertexNotNull  p;
  return face_iterator<NotNull>(p,topology_.faces_begin(),topology_.faces_end());
}

template<>
inline
Mesh<1,serial_mode>::template face_iterator<NotNull>
Mesh<1,serial_mode>::faces_end()
{
  VertexNotNull  p;
  return face_iterator<NotNull>(p,topology_.faces_end(),topology_.faces_end());
}


  template<>
  template<>
  struct
  Mesh<1,serial_mode>::face_iterator<Active>: public biblsimpp_filter_iterator<VertexActive,
                                                                                Mesh<1,serial_mode>::face_iterator_impl>
  {

     typedef typename Mesh<1,serial_mode>::face_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<VertexActive,iterator_impl>::difference_type difference_type;


    face_iterator()
    :
    biblsimpp_filter_iterator<VertexActive,iterator_impl>(){}

    face_iterator(VertexActive p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<VertexActive,iterator_impl>(p,begin,end){}

  };

template<>
inline
Mesh<1,serial_mode>::template face_iterator<Active>
Mesh<1,serial_mode>::active_faces_begin()
{

VertexActive p;
return face_iterator<Active>(p,topology_.faces_begin(),topology_.faces_end());

}

template<>
inline
Mesh<1,serial_mode>::template face_iterator<Active>
Mesh<1,serial_mode>::active_faces_end()
{

VertexActive p;
return face_iterator<Active>(p,topology_.faces_end(),topology_.faces_end());

}

 template<>
  template<>
  struct
  Mesh<1,serial_mode>::face_iterator<ActiveBoundaryObject>: public biblsimpp_filter_iterator<VertexActiveBoundary,
                                                                                Mesh<1,serial_mode>::face_iterator_impl>
  {

     typedef typename Mesh<1,serial_mode>::face_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<VertexActiveBoundary,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<VertexActiveBoundary,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<VertexActiveBoundary,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<VertexActiveBoundary,iterator_impl>::difference_type difference_type;


    face_iterator()
    :
    biblsimpp_filter_iterator<VertexActiveBoundary,iterator_impl>(){}

    face_iterator(VertexActiveBoundary p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<VertexActiveBoundary,iterator_impl>(p,begin,end){}

  };

template<>
inline
Mesh<1,serial_mode>::template face_iterator<ActiveBoundaryObject>
Mesh<1,serial_mode>::active_boundary_faces_begin()
{

  VertexActiveBoundary p;
  return face_iterator<ActiveBoundaryObject>(p,topology_.faces_begin(),topology_.faces_end());
}


template<>
inline
Mesh<1,serial_mode>::template face_iterator<ActiveBoundaryObject>
Mesh<1,serial_mode>::active_boundary_faces_end()
{

  VertexActiveBoundary p;
  return face_iterator<ActiveBoundaryObject>(p,topology_.faces_end(),topology_.faces_end());
}


//=====================================================================================


template<int dim>
template<typename PREDICATE>
struct
Mesh<dim,serial_mode>::const_face_iterator: public
               biblsimpp_filter_iterator<PREDICATE,
                                        typename Mesh<dim,serial_mode>::const_face_iterator_impl>
  {
     typedef typename Mesh<dim,serial_mode>::const_face_iterator_impl iterator_impl;
     typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::value_type value_type;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::reference reference;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::pointer pointer;
     //typedef typename biblsimpp_filter_iterator<PREDICATE,iterator_impl>::difference_type difference_type;

     const_face_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}


    //template<typename PREDICATE>
    const_face_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

  };


template<int dim>
typename Mesh<dim,serial_mode>::template const_face_iterator<NotNull>
Mesh<dim,serial_mode>::faces_begin()const
{
  NotNull  p;
  return const_face_iterator<NotNull>(p,topology_.faces_begin(),topology_.faces_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_face_iterator<NotNull>
Mesh<dim,serial_mode>::faces_end()const
{
  NotNull  p;
  return const_face_iterator<NotNull>(p,topology_.faces_end(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<Active>
Mesh<spacedim,serial_mode>::active_faces_begin()const
{

Active p;
return const_face_iterator<Active >(p,topology_.faces_begin(),topology_.faces_end());

}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<Active  >
Mesh<spacedim,serial_mode>::active_faces_end()const
{

Active p;
return const_face_iterator<Active >(p,topology_.faces_end(),topology_.faces_end());

}



template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<ActiveBoundaryObject>
Mesh<spacedim,serial_mode>::active_boundary_faces_begin()const
{

  ActiveBoundaryObject p;
  return const_face_iterator<ActiveBoundaryObject>(p,topology_.faces_begin(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<ActiveBoundaryObject>
Mesh<spacedim,serial_mode>::active_boundary_faces_end()const
{

  ActiveBoundaryObject p;
  return const_face_iterator<ActiveBoundaryObject >(p,topology_.faces_end(),topology_.faces_end());
}







}

}

#endif // MESH_H
