#ifndef MESH_H
#define MESH_H

#include "biblsimpp_mesh_definition.h"
#include "biblsimpp_mesh_topology.h"
#include "biblsimpp_mesh_connectivity.h"
#include "biblsimpp.h"
#include "biblsimpp_element_type.h"
#include "biblsimpp_iterator.h"
#include "biblsimpp_predicates.h"
#include "biblsimpp_geom_point.h"


namespace numengine
{

  template<int spacedim> class Node;
  template<int spacedim> class Element;
  template<int dim,int topodim> class FaceElement; 
  
  template<int dim>
  struct NotNullNode:public NotNull<Node<dim> >{};
  
  template<int dim>
  struct VertexNode:public IsVertex<Node<dim> >{};
  
  template<int dim>
  struct NotNullElement: public NotNull<Element<dim> >{};
  
  template<int dim>
  struct ActiveElement: public IsActive<Element<dim> >{};
  
  
  template<int dim>
  struct NotNullEdge: public NotNull<FaceElement<dim,1> >{};
  
  template<int dim>
  struct ActiveEdge: public IsActive<FaceElement<dim,1> >{};
  
  
  template<>
  struct NotNullEdge<1>: public NotNull<FaceElement<1,0> >{};
  
  template<>
  struct ActiveEdge<1>: public IsActive<FaceElement<1,0> >{};


  template<int dim>
  struct NotNullFace: public NotNull<FaceElement<dim,2> >{};

  template<int dim>
  struct ActiveFace: public IsActive<FaceElement<dim,2> >{};


  template<>
  struct NotNullFace<2>: public NotNull<FaceElement<2,1> >{};

  template<>
  struct ActiveFace<2>: public IsActive<FaceElement<2,1> >{};


  template<>
  struct NotNullFace<1>: public NotNull<FaceElement<1,0> >{};


  template<>
  struct ActiveFace<1>: public IsActive<FaceElement<1,0> >{};


  template<int dim>
  struct ActiveBoundaryFace: public ActiveBoundaryObject<FaceElement<dim,2> >{};


  template<>
  struct ActiveBoundaryFace<2> : public ActiveBoundaryObject<FaceElement<2,1> >{};


  template<>
  struct ActiveBoundaryFace<1> : public ActiveBoundaryObject<FaceElement<1,0> >{};
  
  
/**
  * \detailed A class that represents a mesh. It is templatized
  * by the storage policy we use as well as the parallel method 
  * we use.
  */
  
template<int spacedim>
class Mesh<spacedim,serial_mode>
{
  
public:
    
    typedef typename MeshTopology<spacedim>::edge_ptr edge_ptr;
    typedef typename MeshTopology<spacedim>::face_ptr face_ptr;
    typedef typename MeshTopology<spacedim>::const_edge_ptr const_edge_ptr;
    typedef typename MeshTopology<spacedim>::const_face_ptr const_face_ptr;    

     /**
       *\detailed default ctor
       */
     Mesh();

     /**
       *\detailed dtor
       */
    ~Mesh();
     
     /**
       *\detailed reserve memory for n nodes
       */
     void reserve_n_nodes(uint_t n){topology_.reserve_n_nodes(n);}
     
     /**
       *\detailed reserve memory for n elements
       */
     void reserve_n_elements(uint_t n){topology_.reserve_n_elements(n);}
     
     
     /**
       *\detailed reserve memory for n edges
       */
     void reserve_n_edges(uint_t n){topology_.reserve_n_edges(n);}

     /**
       *\detailed reserve memory for n faces
       */
     void reserve_n_faces(uint_t n){topology_.reserve_n_faces(n);}
     
     /**
       *\detailed create a vertex by passing in the coordinates
       *of the vertex. This will create a Node so optionally we can
       *pass in the global id and the processor id of the created node
       */
     Node<spacedim>* create_vertex(const GeomPoint<spacedim>& point, 
                                   uint_t global_id=biblSimPP::invalid_uint(),
                                   uint_t pid = 0);
                                   
     /**
       *\detailed create a Node
       */
     Node<spacedim>* create_node(const GeomPoint<spacedim>& point,
                                 uint_t global_id=biblSimPP::invalid_uint(),
                                 uint_t pid = 0);
                                                       
     /**
       *\detailed create an Element and get back the pointer
       */
     Element<spacedim>* create_element(ElementType::sub_type t,
                                       uint_t global_id=biblSimPP::invalid_uint(),
                                       uint_t pid = 0);
                                       
                                       
     /**
       *\detailed create an edge and get back the pointer
       */
     edge_ptr  create_edge(ElementType::sub_type t,
                           uint_t global_id=biblSimPP::invalid_uint(),
                           uint_t pid = 0);


    /**
       *\detailed create a face and get back the pointer
       */
    face_ptr create_face(ElementType::sub_type t,
                         uint_t global_id=biblSimPP::invalid_uint(),
                         uint_t pid=0);

    /**
      * \detailed set the number of boundaries for the mesh
      */
    void set_n_boundaries(uint_t nb){n_boundaries_ = nb;}
                                                                    
   /**
     *\detailed read/write acces to the n-th node
     */
     Node<spacedim>* node(uint_t n){return topology_.node(n);}
          
   /**
     *\detailed read acces to the n-th node
     */
     
    const Node<spacedim>* node(uint_t n)const{return topology_.node(n);}
    
    /**
      *\detailed read/write access to the n-th element
      */
    Element<spacedim>* element(uint_t e){return topology_.element(e);}
    
    /**
      *\detailed read access to the n-th element
      */
    const Element<spacedim>* element(uint_t e)const{return topology_.element(e);}
   
     /**
       *\detailed how many vertices the mesh has
       */
     uint_t n_vertices()const;
     
     /**
       *\detailed how many nodes the mesh has
       */
     uint_t n_nodes()const{return topology_.n_nodes();}
      
     /**
       *\detailed how many elements the mesh has
       */
     uint_t n_elements()const{return topology_.n_elements();}
     
     /**
       *\detailed how many edges the mesh has
       */
     uint_t n_edges()const{return topology_.n_edges();}

     /**
       *\detailed how many faces the mesh has
       */
     uint_t n_faces()const{return topology_.n_faces();}


     /**
       * get the active faces of the mesh
       */
     uint_t n_active_faces()const;
     
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
     uint_t n_boundaries()const{return n_boundaries_;}

    /**
      *\detailed a generic dummy predicate for the Nodes
      */
    typedef biblsimpp_implementation::PredicateBase<Node<spacedim>>     NodePredicate;
    typedef biblsimpp_implementation::PredicateBase<Element<spacedim>>  ElementPredicate;

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
    node_iterator<NotNullNode<spacedim>  > nodes_begin();
    node_iterator<NotNullNode<spacedim>  > nodes_end();

    const_node_iterator<NotNullNode<spacedim>  > nodes_begin()const;
    const_node_iterator<NotNullNode<spacedim>  > nodes_end()const;

    //start-end over the vertices of the mesh
    node_iterator<VertexNode<spacedim> > nodes_vertices_begin();
    node_iterator<VertexNode<spacedim> > nodes_vertices_end();

    const_node_iterator<VertexNode<spacedim> > nodes_vertices_begin()const;
    const_node_iterator<VertexNode<spacedim> > nodes_vertices_end()const;

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
     element_iterator<NotNullElement<spacedim>  > elements_begin();
     element_iterator<NotNullElement<spacedim>  > elements_end();

     const_element_iterator<NotNullElement<spacedim>  > elements_begin()const;
     const_element_iterator<NotNullElement<spacedim>  > elements_end()const;

     //start-end over all active elements of the mesh
     element_iterator<ActiveElement<spacedim>  > active_elements_begin();
     element_iterator<ActiveElement<spacedim>  > active_elements_end();

     const_element_iterator<ActiveElement<spacedim>  > active_elements_begin()const;
     const_element_iterator<ActiveElement<spacedim>  > active_elements_end()const;


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
     edge_iterator<NotNullEdge<spacedim>  > edges_begin();
     edge_iterator<NotNullEdge<spacedim>  > edges_end();

     const_edge_iterator<NotNullEdge<spacedim>  > edges_begin()const;
     const_edge_iterator<NotNullEdge<spacedim>  > edges_end()const;

     //start-end over all active elements of the mesh
     edge_iterator<ActiveEdge<spacedim>  > active_edges_begin();
     edge_iterator<ActiveEdge<spacedim>  > active_edges_end();

     const_edge_iterator<ActiveEdge<spacedim>> active_edges_begin()const;
     const_edge_iterator<ActiveEdge<spacedim>> active_edges_end()const;


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
     face_iterator<NotNullFace<spacedim>  > faces_begin();
     face_iterator<NotNullFace<spacedim>  > faces_end();

     const_face_iterator<NotNullFace<spacedim>  > faces_begin()const;
     const_face_iterator<NotNullFace<spacedim>  > faces_end()const;

     //start-end over all active elements of the mesh
     face_iterator<ActiveFace<spacedim>  > active_faces_begin();
     face_iterator<ActiveFace<spacedim>  > active_faces_end();

     const_face_iterator<ActiveFace<spacedim>  > active_faces_begin()const;
     const_face_iterator<ActiveFace<spacedim>  > active_faces_end()const;

     //start-end over all elements of the mesh
     face_iterator<ActiveBoundaryFace<spacedim>  > active_boundary_faces_begin();
     face_iterator<ActiveBoundaryFace<spacedim>  > active_boundary_faces_end();

     const_face_iterator<ActiveBoundaryFace<spacedim>  > active_boundary_faces_begin()const;
     const_face_iterator<ActiveBoundaryFace<spacedim>  > active_boundary_faces_end()const;


     /**
      *\detailed the object that represents the iterator for the elements
      */
    typedef typename std::vector<face_ptr >::iterator face_iterator_impl;
    typedef typename std::vector<face_ptr >::const_iterator const_face_iterator_impl;

    

private:
    
   /**
     *\detailed the number of boundaries of the mesh
     */
   uint_t n_boundaries_;
    
    
    /**
      *\detailed describes how the various mesh
      * entities are connected
      */
    MeshTopology<spacedim> topology_;
    
  };
  
  

  
template<int dim>
template<typename PREDICATE>
struct
Mesh<dim,serial_mode>::node_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                      typename Mesh<dim,serial_mode>::node_iterator_impl>
{

      typedef typename Mesh<dim,serial_mode>::node_iterator_impl iterator_impl;

      node_iterator()
          :
      biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}

      node_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                        :
                        biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

};


template<int dim>
typename Mesh<dim,serial_mode>::template node_iterator<NotNullNode<dim>  >
Mesh<dim,serial_mode>::nodes_begin()
{
  NotNullNode<dim>  p;
  return node_iterator<NotNullNode<dim> >(p,topology_.nodes_begin(),topology_.nodes_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template node_iterator<NotNullNode<dim> >
Mesh<dim,serial_mode>::nodes_end()
{
  NotNullNode<dim>  p;
  return node_iterator<NotNullNode<dim> >(p,topology_.nodes_end(),topology_.nodes_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template node_iterator<VertexNode<dim> >
Mesh<dim,serial_mode>::nodes_vertices_begin()
{
 VertexNode<dim> p;
 return node_iterator<VertexNode<dim> >(p,topology_.nodes_begin(),topology_.nodes_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template node_iterator<VertexNode<dim> >
Mesh<dim,serial_mode>::nodes_vertices_end()
{
  VertexNode<dim> p;
  return node_iterator<VertexNode<dim> >(p,topology_.nodes_end(),topology_.nodes_end());
}

template<int dim>
template<typename PREDICATE>
struct
Mesh<dim,serial_mode>::const_node_iterator: public
             biblsimpp_filter_iterator<PREDICATE,
                                      typename Mesh<dim,serial_mode>::const_node_iterator_impl>
{

    typedef typename Mesh<dim,serial_mode>::const_node_iterator_impl iterator_impl;

    const_node_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}

    const_node_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                        :
                        biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

};


template<int dim>
typename Mesh<dim,serial_mode>::template const_node_iterator<NotNullNode<dim> >
Mesh<dim,serial_mode>::nodes_begin()const
{
  NotNullNode<dim>  p;
  return const_node_iterator<NotNullNode<dim> >(p,topology_.nodes_begin(),topology_.nodes_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_node_iterator<NotNullNode<dim> >
Mesh<dim,serial_mode>::nodes_end()const
{
 NotNullNode<dim>  p;
 return const_node_iterator<NotNullNode<dim> >(p,topology_.nodes_end(),topology_.nodes_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_node_iterator<VertexNode<dim> >
Mesh<dim,serial_mode>::nodes_vertices_begin()const
{
 VertexNode<dim> p;
 return const_node_iterator<VertexNode<dim> >(p,topology_.nodes_begin(),topology_.nodes_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_node_iterator<VertexNode<dim> >
Mesh<dim,serial_mode>::nodes_vertices_end()const
{
  VertexNode<dim> p;
  return const_node_iterator<VertexNode<dim> >(p,topology_.nodes_end(),topology_.nodes_end());
}




template<int dim>
template<typename PREDICATE>
struct
Mesh<dim,serial_mode>::element_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                         typename Mesh<dim,serial_mode>::element_iterator_impl>
{

   typedef typename Mesh<dim,serial_mode>::element_iterator_impl iterator_impl;

   element_iterator()
       :
   biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}


  //template<typename PREDICATE>
  element_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                        :
                        biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}

};
  
template<int dim>
typename Mesh<dim,serial_mode>::template element_iterator<NotNullElement<dim> >
Mesh<dim,serial_mode>::elements_begin()
{
  NotNullElement<dim>  p;
  return element_iterator<NotNullElement<dim> >(p,topology_.elements_begin(),topology_.elements_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template element_iterator<NotNullElement<dim> >
Mesh<dim,serial_mode>::elements_end()
{
  NotNullElement<dim>  p;
  return element_iterator<NotNullElement<dim> >(p,topology_.elements_end(),topology_.elements_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template element_iterator<ActiveElement<spacedim>  > 
Mesh<spacedim,serial_mode>::active_elements_begin()
{

ActiveElement<spacedim> p;
return element_iterator<ActiveElement<spacedim> >(p,topology_.elements_begin(),topology_.elements_end());
  
}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template element_iterator<ActiveElement<spacedim>  > 
Mesh<spacedim,serial_mode>::active_elements_end()
{

ActiveElement<spacedim> p;
return element_iterator<ActiveElement<spacedim> >(p,topology_.elements_end(),topology_.elements_end());
  
}
    
   
    

  template<int dim>
  template<typename PREDICATE>
  struct 
  Mesh<dim,serial_mode>::const_element_iterator: public 
               biblsimpp_filter_iterator<PREDICATE,
                                        typename Mesh<dim,serial_mode>::const_element_iterator_impl>
  {
     typedef typename Mesh<dim,serial_mode>::const_element_iterator_impl iterator_impl;
     //typedef typename Mesh<dim,serial_mode>::ElementPredicate Predicate;
    
     const_element_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}
   
  
    //template<typename PREDICATE>
    const_element_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}
   
  };
  
  
template<int dim>
typename Mesh<dim,serial_mode>::template const_element_iterator<NotNullElement<dim> >
Mesh<dim,serial_mode>::elements_begin()const
{
  NotNullElement<dim>  p;
  return const_element_iterator<NotNullElement<dim> >(p,topology_.elements_begin(),topology_.elements_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_element_iterator<NotNullElement<dim> >
Mesh<dim,serial_mode>::elements_end()const
{
  NotNullElement<dim>  p;
  return const_element_iterator<NotNullElement<dim> >(p,topology_.elements_end(),topology_.elements_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_element_iterator<ActiveElement<spacedim>  > 
Mesh<spacedim,serial_mode>::active_elements_begin()const
{

ActiveElement<spacedim> p;
return const_element_iterator<ActiveElement<spacedim> >(p,topology_.elements_begin(),topology_.elements_end());
  
}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_element_iterator<ActiveElement<spacedim>  > 
Mesh<spacedim,serial_mode>::active_elements_end()const
{

ActiveElement<spacedim> p;
return const_element_iterator<ActiveElement<spacedim> >(p,topology_.elements_end(),topology_.elements_end());
  
}

//Edge iterator

template<int dim>
  template<typename PREDICATE>
  struct 
  Mesh<dim,serial_mode>::edge_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                        typename Mesh<dim,serial_mode>::edge_iterator_impl>
  {
  
     typedef typename Mesh<dim,serial_mode>::edge_iterator_impl iterator_impl;
     
    
    edge_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}
   
    edge_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}
   
  };
  
template<int dim>
typename Mesh<dim,serial_mode>::template edge_iterator<NotNullEdge<dim> >
Mesh<dim,serial_mode>::edges_begin()
{
  NotNullEdge<dim>  p;
  return edge_iterator<NotNullEdge<dim> >(p,topology_.edges_begin(),topology_.edges_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template edge_iterator<NotNullEdge<dim> >
Mesh<dim,serial_mode>::edges_end()
{
  NotNullEdge<dim>  p;
  return edge_iterator<NotNullEdge<dim> >(p,topology_.edges_end(),topology_.edges_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template edge_iterator<ActiveEdge<spacedim>  > 
Mesh<spacedim,serial_mode>::active_edges_begin()
{

ActiveEdge<spacedim> p;
return edge_iterator<ActiveEdge<spacedim> >(p,topology_.edges_begin(),topology_.edges_end());
  
}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template edge_iterator<ActiveEdge<spacedim>  >  
Mesh<spacedim,serial_mode>::active_edges_end()
{

ActiveEdge<spacedim> p;
return edge_iterator<ActiveEdge<spacedim> >(p,topology_.edges_end(),topology_.edges_end());
  
}


template<int dim>
  template<typename PREDICATE>
  struct 
  Mesh<dim,serial_mode>::const_edge_iterator: public 
               biblsimpp_filter_iterator<PREDICATE,
                                        typename Mesh<dim,serial_mode>::const_edge_iterator_impl>
  {
     typedef typename Mesh<dim,serial_mode>::const_edge_iterator_impl iterator_impl;
     
     const_edge_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}
   
  
    //template<typename PREDICATE>
    const_edge_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}
   
  };
  
  
template<int dim>
typename Mesh<dim,serial_mode>::template const_edge_iterator<NotNullEdge<dim> >
Mesh<dim,serial_mode>::edges_begin()const
{
  NotNullEdge<dim>  p;
  return const_edge_iterator<NotNullEdge<dim> >(p,topology_.edges_begin(),topology_.edges_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_edge_iterator<NotNullEdge<dim> >
Mesh<dim,serial_mode>::edges_end()const
{
  NotNullEdge<dim>  p;
  return const_edge_iterator<NotNullEdge<dim> >(p,topology_.edges_end(),topology_.edges_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_edge_iterator<ActiveEdge<spacedim>  > 
Mesh<spacedim,serial_mode>::active_edges_begin()const
{

ActiveEdge<spacedim> p;
return const_edge_iterator<ActiveEdge<spacedim> >(p,topology_.edges_begin(),topology_.edges_end());
  
}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_edge_iterator<ActiveEdge<spacedim>  > 
Mesh<spacedim,serial_mode>::active_edges_end()const
{

ActiveEdge<spacedim> p;
return const_edge_iterator<ActiveEdge<spacedim> >(p,topology_.edges_end(),topology_.edges_end());
  
}


//Face iterator

template<int dim>
  template<typename PREDICATE>
  struct 
  Mesh<dim,serial_mode>::face_iterator: public biblsimpp_filter_iterator<PREDICATE,
                                                                        typename Mesh<dim,serial_mode>::face_iterator_impl>
  {
  
     typedef typename Mesh<dim,serial_mode>::face_iterator_impl iterator_impl;
     
    
    face_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}
   
    face_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}
   
  };
  
template<int dim>
typename Mesh<dim,serial_mode>::template face_iterator<NotNullFace<dim> >
Mesh<dim,serial_mode>::faces_begin()
{
  NotNullFace<dim>  p;
  return face_iterator<NotNullFace<dim> >(p,topology_.faces_begin(),topology_.faces_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template face_iterator<NotNullFace<dim> >
Mesh<dim,serial_mode>::faces_end()
{
  NotNullFace<dim>  p;
  return face_iterator<NotNullFace<dim> >(p,topology_.faces_end(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<ActiveFace<spacedim>  > 
Mesh<spacedim,serial_mode>::active_faces_begin()
{

ActiveFace<spacedim> p;
return face_iterator<ActiveFace<spacedim> >(p,topology_.faces_begin(),topology_.faces_end());
  
}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<ActiveFace<spacedim>  >  
Mesh<spacedim,serial_mode>::active_faces_end()
{

ActiveFace<spacedim> p;
return face_iterator<ActiveFace<spacedim> >(p,topology_.faces_end(),topology_.faces_end());
  
}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<ActiveBoundaryFace<spacedim>  >
Mesh<spacedim,serial_mode>::active_boundary_faces_begin()
{

  ActiveBoundaryFace<spacedim> p;
  return face_iterator<ActiveBoundaryFace<spacedim> >(p,topology_.faces_begin(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template face_iterator<ActiveBoundaryFace<spacedim>  >
Mesh<spacedim,serial_mode>::active_boundary_faces_end()
{

  ActiveBoundaryFace<spacedim> p;
  return face_iterator<ActiveBoundaryFace<spacedim> >(p,topology_.faces_end(),topology_.faces_end());
}


template<int dim>
template<typename PREDICATE>
struct 
Mesh<dim,serial_mode>::const_face_iterator: public 
               biblsimpp_filter_iterator<PREDICATE,
                                        typename Mesh<dim,serial_mode>::const_face_iterator_impl>
  {
     typedef typename Mesh<dim,serial_mode>::const_face_iterator_impl iterator_impl;
     
     const_face_iterator():biblsimpp_filter_iterator<PREDICATE,iterator_impl>(){}
   
  
    //template<typename PREDICATE>
    const_face_iterator(PREDICATE p,iterator_impl begin ,iterator_impl end)
                          :
                          biblsimpp_filter_iterator<PREDICATE,iterator_impl>(p,begin,end){}
   
  };
  
  
template<int dim>
typename Mesh<dim,serial_mode>::template const_face_iterator<NotNullFace<dim> >
Mesh<dim,serial_mode>::faces_begin()const
{
  NotNullFace<dim>  p;
  return const_face_iterator<NotNullFace<dim> >(p,topology_.faces_begin(),topology_.faces_end());
}

template<int dim>
typename Mesh<dim,serial_mode>::template const_face_iterator<NotNullFace<dim> >
Mesh<dim,serial_mode>::faces_end()const
{
  NotNullFace<dim>  p;
  return const_face_iterator<NotNullFace<dim> >(p,topology_.faces_end(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<ActiveFace<spacedim>  > 
Mesh<spacedim,serial_mode>::active_faces_begin()const
{

ActiveFace<spacedim> p;
return const_face_iterator<ActiveFace<spacedim> >(p,topology_.faces_begin(),topology_.faces_end());
  
}

template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<ActiveFace<spacedim>  > 
Mesh<spacedim,serial_mode>::active_faces_end()const
{

ActiveFace<spacedim> p;
return const_face_iterator<ActiveFace<spacedim> >(p,topology_.faces_end(),topology_.faces_end());
  
}



template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<ActiveBoundaryFace<spacedim>  >
Mesh<spacedim,serial_mode>::active_boundary_faces_begin()const
{

  ActiveBoundaryFace<spacedim> p;
  return const_face_iterator<ActiveBoundaryFace<spacedim> >(p,topology_.faces_begin(),topology_.faces_end());
}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::template const_face_iterator<ActiveBoundaryFace<spacedim>  >
Mesh<spacedim,serial_mode>::active_boundary_faces_end()const
{

  ActiveBoundaryFace<spacedim> p;
  return const_face_iterator<ActiveBoundaryFace<spacedim> >(p,topology_.faces_end(),topology_.faces_end());
}

}//libsimpp
#endif
