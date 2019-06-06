#include "biblsimpp_serial_mesh.h"
#include "biblsimpp_node.h"
#include "biblsimpp_element.h"
#include "biblsimpp_config.h"

//C++
#include<algorithm>

namespace biblsimpp
{

template<int spacedim>
Mesh<spacedim,serial_mode>::Mesh()
               :
               n_boundaries_(0),
               topology_()
{}

template<int spacedim>
Mesh<spacedim,serial_mode>::~Mesh()
{}


template<int spacedim>
Node<spacedim>* 
Mesh<spacedim,serial_mode>::create_vertex(const GeomPoint<spacedim>& point, 
                                         size_type global_id,size_type pid)
{

  Node<spacedim>* n= new Node<spacedim>(point,global_id,pid,this);
  n->make_vertex();
  
  topology_.add_node(n);
  
  return n;

}

template<int spacedim>
Node<spacedim>* 
Mesh<spacedim,serial_mode>::create_node(const GeomPoint<spacedim>& point,
                                        size_type global_id,
                                        size_type pid)
{

  Node<spacedim>* n= new Node<spacedim>(point,global_id,pid,this);
  topology_.add_node(n);
  return n;
}


template<int spacedim>
Element<spacedim>* 
Mesh<spacedim,serial_mode>::create_element(ElementType::sub_type t,
                                           size_type global_id,
                                           size_type pid)
{

  Element<spacedim>* e = ElementBuilder<spacedim>::build_element(t);
  
  e->set_mesh(*this);
  e->set_id(global_id);
  e->set_processor_id(pid);

  topology_.add_element(e);
  
  return e;

}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::edge_ptr
Mesh<spacedim,serial_mode>::create_edge(ElementType::sub_type t,
                                        size_type global_id,
                                        size_type pid )
{

typename Mesh<spacedim,serial_mode>::edge_ptr edge = ElementBuilder<spacedim>::build_edge(t);
edge->set_mesh(*this);
edge->set_id(global_id);
edge->set_processor_id(pid);

topology_.add_edge(edge);

return edge;

}


template<int spacedim>
typename Mesh<spacedim,serial_mode>::face_ptr
Mesh<spacedim,serial_mode>::create_face(ElementType::sub_type t,
                                        size_type global_id,
                                        size_type pid )
{

typename Mesh<spacedim,serial_mode>::face_ptr edge = ElementBuilder<spacedim>::build_face(t);
edge->set_mesh(*this);
edge->set_id(global_id);
edge->set_processor_id(pid);

topology_.add_face(edge);

return edge;

}

template<int spacedim>
size_type
Mesh<spacedim,serial_mode>::n_vertices()const
{


  size_type nvertices = std::count_if(topology_.nodes_begin(),
                                      topology_.nodes_end(),
                                      [](const Node<spacedim>* n){return n->is_vertex();});
   
   return nvertices;
}


template<int spacedim>
size_type
Mesh<spacedim,serial_mode>::n_active_faces()const
{

 

 return std::count_if(topology_.faces_begin(),
                      topology_.faces_end(),
                      [](typename Mesh<spacedim,serial_mode>::const_face_ptr face){return face->is_active();});
 //return static_cast<size_type>(std::distance(active_faces_begin(),active_faces_end());

}

#ifdef BIBLSIMPP_DIM_1
 template class Mesh<1,serial_mode>;
#endif

#ifdef BIBLSIMPP_DIM_2
 template class Mesh<2,serial_mode>;
#endif

#ifdef BIBLSIMPP_DIM_3
 template class Mesh<3,serial_mode>;
#endif

}//libsimpp
