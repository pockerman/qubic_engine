#ifndef MESH_ENTITY_H
#define MESH_ENTITY_H

#include "parframe/base/types.h"
#include "parframe/base/kernel_consts.h"
#include<ostream>

namespace numengine
{

//Forward declarations
template<int spacedim> class Mesh;

/**
  * \detailed A MeshEntity object provides a view of a given meshe entity \p(d,i). 
  * Examples of mesh entities are Node, Edges, Faces, Facets, and Elements. We do not store
  * the mesh entities themselves in our Mesh data structure instead these may be generated from a 
  * given pair (d,i). This class is meant to provide a convenient interface for accessing mesh data
  * by using mesh iterators. The interface of this class provides functionality for enquiring about the
  * topological dimension of the object its index i and its set of incidence relations i.e. the other mesh
  * entities of a given topological dimension \p d' that this object is connected to. 
  * Subclasses of this class are the Node, Edge, Face, Facet and Cell 
  */

template<int spacedim,int topodim>
class MeshEntity
{

public:

  static const int space_dim = spacedim;
  static const int topological_dim = topodim;
  
  /**
    * \detailed a useful enumeration describing the
    * types of MeshEntity 
    */
  enum class Type{NODE, EDGE, FACE, FACET, CELL};
    
protected:

   /**
     * \detailed default ctor creates an invalid object
     */     
     MeshEntity();
     
     /**
       * \detailed ctor create an object by passing down the \p Mesh
       * the topological dimension the id and optionally the processor id
       */
     MeshEntity(const Mesh<spacedim>* m, kernel::uint_t id, kernel::uint_t pid=0);
 
public:
     
     /**
       *\detailed dtor pure virtual so that derived classes
       *should be forced to provide their own dtor
       */
    virtual ~MeshEntity(){}
       
     /**
       * \detailed set the Mesh this entity belongs to
       */
     void set_mesh(const Mesh<spacedim>& m){mesh_=&m;}
     
     /**
       * \detailed set the id for this entity
       */
     void set_id(kernel::uint_t id){id_ = id;}
     
     /**
       * \detailed set the processor id for this entity
       */
     void set_processor_id(kernel::uint_t pid){proc_id_ = pid;}
     
     /**
       * \detailed get the Mesh this entity belongs to
       */
      const Mesh<spacedim>* mesh()const{return mesh_;}
      
      /**
        * \detailed get the global id for this entity
        */
      kernel::uint_t id()const{return id_;}
      
      /**
        * \detailed get the processor id for this entity
        */
      kernel::uint_t pid()const{return proc_id_;}
      
      /**
        *\detailed returns true iff id_ != LibSimPP::invalid_uint()
        */
       bool has_valid_id()const{return id_ != parframe::kernel_consts::invalid_size_type();}
      
       /**
         *\detailed print the information for the MeshEntity
         */

       virtual std::ostream& print_mesh_entity_info(std::ostream& out)const;
      
protected:

    //copy constructor and assignement operators
    //are protected so that derived classes can call them
    //this class is templatized so the compiler sees
    //MeshEntity<2,0> and MeshEntity<2,1> beign different
    //hence strictly there is no need to do so in order to avoid
    //partial assignements in derived classes since the following
    //will not compile Edge<2> e; Node<2> p = e; since Edge is
    //MeshEntity<2,1> and  Node is a MeshEntity<2,0> this is because
    //copy assignement and copy construction should call the base class
    //related operators. Anyway, we place these here to make it more obvious

    /**
      *\detailed copy ctor
      */
     MeshEntity(const MeshEntity& o);

    /**
      *\detailed copy assignement
      */
     MeshEntity& operator=(const MeshEntity& o);
     
     
private:

    const Mesh<spacedim>* mesh_;
 
    /**
      * \detailed the global id of the object
      */
    kernel::uint_t id_;

    /**
      *\detailed the processor id that this object belongs to
      */
    kernel::uint_t proc_id_;
 
};

template<int spacedim,int topodim>
inline
MeshEntity<spacedim,topodim>::MeshEntity()
	                    :
	                    mesh_(nullptr),
                            id_(parframe::kernel_consts::invalid_size_type()),
	                    proc_id_(0)
{}

template<int spacedim,int topodim>
inline
MeshEntity<spacedim,topodim>::MeshEntity(const Mesh<spacedim>* m, kernel::uint_t id, kernel::uint_t pid)
	                     :
	                    mesh_(m),
	                    id_(id),
	                    proc_id_(pid)
{}

template<int spacedim,int topodim>
inline
MeshEntity<spacedim,topodim>::MeshEntity(const MeshEntity<spacedim, topodim>& o)
                             :
                             mesh_(o.mesh_),
                             id_(o.id_),
                             proc_id_(o.proc_id_)
                             {}

template<int spacedim,int topodim>
inline
MeshEntity<spacedim,topodim>&
MeshEntity<spacedim,topodim>::operator=(const MeshEntity<spacedim,topodim>& o)
{

  if(this==&o){
      return *this;
  }
  
  mesh_ = o.mesh_;
  id_   = o.id_;
  proc_id_ = o.proc_id_;
  
  return *this;
}

template<int spacedim,int topodim>
inline
std::ostream& 
MeshEntity<spacedim,topodim>::print_mesh_entity_info(std::ostream &out)const
{
  out<<" Spatial dim=  "<<spacedim<<std::endl;
  out<<" Topodim=      "<<topodim<<std::endl;
  out<<" Global id=    "<<id_<<std::endl;
  out<<" Processor id= "<<proc_id_<<std::endl;
  return out;
}

template<int spacedim,int topodim>
inline
std::ostream& operator<<(std::ostream& out,const MeshEntity<spacedim,topodim>& o)
{
  return o.print_mesh_entity_info(out);
}

}//numengine
#endif
