#ifndef NODE_H
#define NODE_H

//#include "numengine/geom/face_element.h"
#include "parframe/base/types.h"

//C++
#include<ostream>
#include<functional>

namespace numengine
{

//Forward declarations
template<int spacedim,typename PARALLEL_MODE> class Mesh;
template<int spacedim> class GeomPoint;
 

/**
  * \detailed A class that represents a Node. A Node
  * is a MeshEntity object and a GeomPoint. It also holds 
  * dofs
  */
  
template<int spacedim>
class Node /*: public FaceElement<spacedim,0>*/
           
{

public:
    
// @}
  /**
   * @name Constructors
   */
// @{

  /**
    * \detailed default ctor
    */
  Node();

  /**
    * \detailed ctor all dim data are assigned the given value
    */
  explicit Node(kernel::uint_t global_id,
                kernel::real_t val=static_cast<kernel::real_t>(0.0),
                kernel::uint_t pid=0);
  
  /**
    *\detailed create by passing a vector of data
    */
  Node(kernel::uint_t global_id,
       const std::vector<kernel::real_t>& data,
       kernel::uint_t pid=0);
  
  /**
    *\detailed create from the given \p point. 
    */     
  Node(const GeomPoint<spacedim>& point,  kernel::uint_t global_id,
       kernel::uint_t pid=0);
  
  /**
    * \detailed copy ctor
    */
  Node(const Node& t);
  
  /**
    *\detailed copy assignement operator
    */
  Node& operator=(const Node& t);
  
  /**
    *\detailed dtor
    */
  ~Node(){}
  
    /**
      *\detailed print the information for the node
      */
    
     std::ostream& print_node_info(std::ostream &out)const;
     
     struct IsNodeId:public std::unary_function<kernel::uint_t,bool>
     {
     
        IsNodeId(kernel::uint_t id):id_(id){}
     
        bool operator()(kernel::uint_t id)
        {
        
          return id == id_;
        
        }
        
        kernel::uint_t id_;
     }; 
};

template<int spacedim>
inline
Node<spacedim>::Node()
{}
               
    
template<int spacedim>
inline
Node<spacedim>::Node(kernel::uint_t global_id,
                     kernel::real_t val,
                     kernel::uint_t pid)
{}

template<int spacedim>
inline
Node<spacedim>::Node(kernel::uint_t global_id,
                     const std::vector<kernel::real_t>& data,
                     kernel::uint_t pid)
{}
                                 
template<int spacedim>
inline
Node<spacedim>::Node(const GeomPoint<spacedim>& point, kernel::uint_t global_id,
                     kernel::uint_t pid)
{}
                
template<int spacedim>
inline
Node<spacedim>::Node(const Node<spacedim>& t)
{}

template<int spacedim>
inline
Node<spacedim>& 
Node<spacedim>::operator=(const Node<spacedim>& o){

  if(this==&o){
      return *this;
  }
  
  return *this;
}

template<int spacedim>
inline
std::ostream& 
Node<spacedim>::print_node_info(std::ostream &out)const{
  //this->print_mesh_entity_info(out);
  return out;
}

template<int dim>
inline
bool operator==(const Node<dim>& n1,const Node<dim>& n2)
{

   //for(int d=0; d<dim; ++d)
    //if(std::fabs(n1[d]-n2[d])>biblSimPP::TOL())
    //  return false;

 return true;

}

template<int dim>
inline
bool operator!=(const Node<dim>& n1,const Node<dim>& n2){
 return !(n1==n2);
}

//specialization for 1D
#ifdef BIBLSIMPP_DIM_1

template<>
class Node<1>: public FaceElement<1,0>
           
{


  public:
    
// @}
  /**
   * @name Constructors
   */
// @{

  /**
    * \detailed default ctor
    */
  Node();

  /**
    * \detailed ctor all dim data are assigned the given value
    */
  explicit Node(size_type global_id,
                real_type val=biblsimpp_value_traits<real_type>::zero(),
                size_type pid=0,
                const Mesh<1,serial_mode>* m = nullptr);
  
  /**
    *\detailed create by passing a vector of data
    */
  Node(size_type global_id,
       const std::vector<real_type>& data,
       size_type pid=0,
       const Mesh<1,serial_mode>* m = nullptr);
  
  
  /**
    *\detailed create from the given array. \p data should have at least size spacedim
    * the first dim components are used
    */
  Node(size_type global_id,
       real_type data[],
       size_type pid=0,
       const Mesh<1,serial_mode>* m = nullptr);
       
  
  /**
    *\detailed create from the given \p point. 
    */     
  Node(const GeomPoint<1>& point, 
       size_type global_id,
       size_type pid=0, const Mesh<1,serial_mode>* m=nullptr);
  
  
  /**
    * \detailed copy ctor
    */
  Node(const Node& t);
  
  
  /**
    *\detailed copy assignement operator
    */
  Node& operator=(const Node& t);
  
  
  /**
    *\detailed dtor
    */
  ~Node(){}
  
  /**
    *\detailed get the nodes of the element that are necessary to
    *have an approximation order \p Order
    */
   virtual void get_order_nodes(NumMethodBase::Order o,
                                std::vector<const Node<1>* >& nodes)const;
  
  /**
    *\detailed print the information for the node
    */
    
  std::ostream& print_node_info(std::ostream &out)const;
     
     
   struct IsNodeId:public std::unary_function<size_type,bool>
   {
     
        IsNodeId(size_type id):id_(id){}
     
        bool operator()(size_type id)
        {
        
          return id == id_;
        
        }
        
        size_type id_;
     
   };


  /**
    *read/write access to the i-th local node
    */
   virtual Node<1>* node(size_type){return this;}
    
    
    /**
      * read access to the i-th local node
      */
   virtual const Node<1>* node(size_type i)const{return this;}

};


//template<int spacedim>
inline
Node<1>::Node()
               :
               FaceElement<1,0>()
               {}
               
    
//template<int spacedim>
inline
Node<1>::Node(size_type global_id,
                     real_type val,
                     size_type pid,
                     const Mesh<1,serial_mode>* m)
               :
               FaceElement<1,0>(global_id,val,pid,m)
               {}

//template<int spacedim>
inline
Node<1>::Node(size_type global_id,
              const std::vector<real_type>& data,
              size_type pid,
              const Mesh<1,serial_mode>* m)
                :
              FaceElement<1,0>(global_id,data,pid,m)
                {}
                
               
//template<int spacedim>
inline
Node<1>::Node(size_type global_id,
                     real_type data[],
                     size_type pid,
                     const Mesh<1,serial_mode>* m)
                 :
                 FaceElement<1,0>(global_id,data,pid,m)
                 {}
                 
//template<int spacedim>
inline
Node<1>::Node(const GeomPoint<1>& point, 
                     size_type global_id,
                     size_type pid, const Mesh<1,serial_mode>* m)
                     :
                     FaceElement<1,0>(point,global_id,pid,m)
                     {}
                
//template<int spacedim>
inline
Node<1>::Node(const Node<1>& t)
               :
               FaceElement<1,0>(t)
               {}

//template<int spacedim>
inline
Node<1>& 
Node<1>::operator=(const Node<1>& o)
{

  if(this==&o)return *this;
  
  this->FaceElement<1,0>::operator=(o);
  return *this;
}

//template<int spacedim>
inline
std::ostream& 
Node<1>::print_node_info(std::ostream &out)const
{

  this->print_mesh_entity_info(out);
  return out;
}

inline
void 
Node<1>::get_order_nodes(NumMethodBase::Order o,
                         std::vector<const Node<1>* >& nodes)const
{

  nodes.clear();
  nodes.push_back(this);

}

#endif

template<int spacedim>
inline
std::ostream & operator << (std::ostream &out, const Node<spacedim> &n)
{
 return n.print_node_info(out); 

}

}//numengine
#endif
