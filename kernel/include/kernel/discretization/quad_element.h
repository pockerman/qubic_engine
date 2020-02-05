#ifndef QUAD_ELEMENT_H
#define QUAD_ELEMENT_H

#include "kernel/base/types.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face.h"
#include "kernel/discretization/quad_implementation.h"


namespace kernel
{

namespace numerics
{

/**
  *\detailed a class that represents quad type
  *elements in MeshGeometry::dim_ spatial dimension. In 2D
  * this object acts like a proper element. In 3D this class
  * has extented functionality making act like a face to Hex elements
  */
template<int dim>
class Quad: public Element<dim>
{

 public:

    typedef typename Element<dim>::node_ptr_t node_ptr_t;
    typedef typename Element<dim>::neighbor_ptr_t neighbor_ptr_t;
 
   /**
     *\detailed default ctor
     */
   explicit Quad(uint_t nnodes=4);
   
   
   explicit Quad(uint_t id, uint_t pid, uint_t n=4);
 

  /**
     *\detailed the total number of nodes for the
     *element
     */
   virtual uint_t n_nodes()const override final{return impl_.n_nodes();}
   
   /**
     *\detailed quads has 4 vertices independent of the
     *dimension
     */
   virtual uint_t n_vertices()const override final{return 4;}

    /// \brief Set the i-th node
    virtual void set_node(uint_t i, node_ptr_t node)override;

    /// \brief Append a  node to the nodes list
    virtual void append_node(node_ptr_t node)override;

    /// \brief Reserve space for nodes
    virtual void reserve_nodes(uint n)override;

    /// \brief Returns the i-th node
    virtual node_ptr_t get_node(uint_t n)override;
   
   /**
     *\detailed quads have 4 neighbors independent of the
     *dimension
     */
   virtual uint_t n_neighbors()const  final{return 4;}

    /// \brief Set the i-th neighbor
    virtual void set_neighbor(uint n, neighbor_ptr_t neigh){}

    /// \brief Reserve space for neighbors
    virtual void reserve_neighbors(uint n){}

    /// \brief Access the n-th neighbor
    virtual neighbor_ptr_t get_neighbor(uint_t n){};
   
   /**
     *\detailed quads have 4 sides independent of the dimension
     */
   virtual uint_t n_faces()const  final{return 4;}
   
   /**
     *\detailed quads have 4 edges independent of the dimension
     */
   virtual uint_t n_edges()const  final{return 4;}
   
   /**
     *\detailed get the volume of the element
     */
   virtual real_t volume()const{}
   
   
   /**
     *\detailed refine the element
     */
   virtual void refine(){}
   
   
   /**
      *\detailed get the vertices global indices for the face-th face of the 
      *element 
      */
    virtual void face_vertices(uint_t face,std::vector<uint_t>& face_vertices)const{}
 
 private:
 
  /**
    *\detailed object that handles the various calculations
    */
  QuadImpl<dim> impl_;
 
};



template<int spacedim>
inline
Quad<spacedim>::Quad(uint_t n)
               :
               Element<spacedim>(),
               impl_(n)
               {}
               
template<int spacedim>
inline
Quad<spacedim>::Quad(uint_t id, uint_t pid, uint_t n)
               :
               Element<spacedim>(id,pid),
               impl_(n)
               {}
               

//specialization for 3D Quad elements act like faces
template<>
class Quad<3>:public Face
{


public:

   /**
     *\detailed default ctor
     */
   explicit Quad(uint_t nnodes=4);
   
   
   explicit Quad(uint_t id, uint_t pid=0,uint_t n=4);
 

  /**
     *\detailed the total number of nodes for the
     *element
     */
   virtual uint_t n_nodes()const {return impl_.n_nodes();}
   
   /**
     *\detailed quads has 4 vertices independent of the
     *dimension
     */
   virtual uint_t n_vertices()const {return 4;}
   
   /**
     *\detailed quads have 4 neighbors independent of the
     *dimension
     */
   virtual uint_t n_neighbors()const {return 4;}
   
   /**
     *\detailed quads have 4 sides independent of the dimension
     */
   virtual uint_t n_faces()const {return 4;}
   
   /**
     *\detailed quads have 4 edges independent of the dimension
     */
   virtual uint_t n_edges()const {return 4;}
   
   /**
     *\detailed get the volume of the element
     */
   virtual real_t volume()const{}
   
   
   /**
     *\detailed refine the element
     */
   virtual void refine(){}
 
 
   /**
      *\detailed get the vertices global indices for the face-th face of the 
      *element 
      */
     virtual void face_vertices(uint_t face,std::vector<uint_t>& face_vertices)const{}
 

 private:
 
  /**
    *\detailed object that handles the various calculations
    */
  QuadImpl<3> impl_;

}; 


inline
Quad<3>::Quad(uint_t n)
               :
               Face(),
               impl_(n)
               {}
               

inline
Quad<3>::Quad(uint_t id, uint_t pid,uint_t n)
               :
               Face(id,pid),
               impl_(n)
               {}              
}
}
#endif
