#ifndef FACE_ELEMENT_H
#define FACE_ELEMENT_H

#include "kernel/base/types.h"
#include "kernel/numerics/dof_object.h"
#include "kernel/discretization/edge_face_selector.h"
#include "kernel/discretization/element_traits.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/base/kernel_consts.h"

#include <array>

namespace kernel
{

namespace numerics
{

//forward declarations  
template<int dim> class Element;
template<int spacedim> class Mesh;
template<int dim,int topodim> class FaceElement;

/*
  \detailed A FaceElement<spacedim,0> represents a point
  in spacedim spatial dimensions. Edges of spatial dimension
  \p spacedim return FaceElement<spacedim,0> ptrs when asked
  about their edges and faces. We do not derive from Element<spacedim,topodim>
  because we want to have this class to behave like a Point and not like an Element.
  There is a specialization for 1D that provides some extended functionality like
  inquiring if the point is on the boundary and on which boundary, which Element
  owns the point and if it is an internal point who is the neighbor 
*/
template<int spacedim>
class FaceElement<spacedim, 0>: public GeomPoint<spacedim>,
                               DoFObject
{

public:


    /**
      * \detailed default ctor
      */
    FaceElement();

    /**
      * \detailed ctor all dim data are assigned the given value
      */
    explicit FaceElement(uint_t global_id,
                         real_t val=0.0,
                         uint_t pid=0);

    /**
      *\detailed create by passing a vector of data
      */
    FaceElement(const std::array<real_t, spacedim>& coords,
                uint_t global_id, uint_t pid=0);

    /**
      *\detailed create from the given \p point.
      */
    FaceElement(const GeomPoint<spacedim>& point,
                uint_t global_id,
                uint_t pid=0);

    /**
      * \detailed copy ctor
      */
    FaceElement(const FaceElement& t);

    /**
      *\detailed copy assignement operator
      */
    FaceElement& operator=(const FaceElement& t);
  
    /// \brief dtor
    virtual ~FaceElement()
    {}


   /**
     * \detailed add a variable to the array that holds the variables that this object
     * manages
     */
  //void add_variable(const std::string& var_name,uint_t n_components)
  //{dof_object_.add_variable(var_name,n_components);}
  
  
  /**
    * \detailed remove the variable \p var_name
    */
    //void remove_variable(const std::string& var_name){dof_object_.remove_variable(var_name);}
 
    /**
      * \detailed set the global dof index for the variable with name \p var_name for the variable
      * component \p comp_idx
      */
    //void set_comp_var_dof_idx(const std::string& var_name, uint_t comp_idx, uint_t dof_idx)
    //{dof_object_.set_comp_var_dof_idx(var_name,comp_idx,dof_idx);}
    
    
    /**
      * \detailed set the dofs for the variable \p var_name
      */
    //void set_var_dofs(const std::string& var_name,const std::vector<Dof>& dofs)
    //{dof_object_.set_var_dofs(var_name,dofs);}
    
    
    /**
      *\detailed invalidate the dofs for the variable \p var_name
      */
    //void invalidate_dofs(const std::string& var_name){dof_object_.invalidate_dofs(var_name);}
    
    
    /**
      * \detailed invaidate all the dofs for all variables
      */
    //void invalidate_dofs(){dof_object_.invalidate_dofs();}
    
    
    /**
      *\detailed clear the list of variables this object handles
      */
    //void clear_dofs(){dof_object_.clear();}
    
    
    /**
      *\detailed make this node a vertex
      */
    void make_vertex(){is_vertex_ = true;}
  
     /**
       *\detailed get the dof index for the component \p comp_idx for the variable \p var_name
       */
   //uint_t comp_var_dof_index(const std::string& var_name,uint_t comp_idx)const
   // {return dof_object_.comp_var_dof_index(var_name,comp_idx);}
   
   
    /**
      * \detailed get the Dof vector for the variable \p var_name 
      */
    //std::vector<Dof> var_dof_vector(const std::string& var_name)const{return dof_object_.var_dof_vector(var_name);}
    
    
    /**
      *\detailed returns true iff the object has variables
      */
    //bool has_variables()const{return dof_object_.has_variables();}
    
    
    /**
      *\detailed returns true iff the variable \p var_name is in the list
      * of variables this object manages
      */
    //bool has_variable(const std::string& var_name)const{return dof_object_.has_variable(var_name);}
    
    
    /**
      *\detailed true iff the dof index for the variable \p var_name and its component
      *\p comp is not LibSimPP::invalid_uint()
      */
    //bool has_valid_dof_idx(const std::string& var_name,uint_t comp)const
    //{return dof_object_.has_valid_dof_idx(var_name,comp);}
    
    /**
      *\detailed returns true iff \p make_vertex has been called on this object
      */
    bool is_vertex()const{return is_vertex_;}
    
    
    bool is_active()const{return true;}
    
    
    /**
     *\detailed print the information for the MeshEntity
     */
    virtual std::ostream& print_mesh_entity_info(std::ostream &out)const;
  
    uint_t get_id()const{return id_;}
    void set_id(uint_t id){id_ = id;}
    bool has_valid_id()const{return id_ != KernelConsts::invalid_size_type();}

  private:
  
  
  /**
    *\detailed an object that handles the dofs
    *for this FaceElement
    */
  //DofObject dof_object_;
  
  
  /**
    *\detailed flag indicating if the node is a vertex
    *default is false
    */
   bool is_vertex_;
   
    
  /**
    *\detailed the boundary indicator of the Node
    */
  uint_t boundary_indicator_;

  uint_t id_;

};

template<int spacedim>
inline
std::ostream& 
FaceElement<spacedim,0>::print_mesh_entity_info(std::ostream &out)const
{
  /*this->print_point_info(out);
  this->MeshEntity<spacedim,0>::print_mesh_entity_info(out);
  dof_object_.print_dof_object_info(out);*/
  
  return out;

}

/**
  *\detailed Explicit instantiation for 1D. In 1D a Node is also an edge/face
  */
  
template<>
class FaceElement<1,0>: public GeomPoint<1>,
                        DoFObject
{

 public:
 
 static const int dim_ = 1;
 static const int topodim_ = 0;



  /**
    * \detailed default ctor
    */
  FaceElement();

  /**
    * \detailed ctor all dim data are assigned the given value
    */
  explicit FaceElement(uint_t global_id,
                       real_t val=0.0,
                       uint_t pid=0);
  
  /**
    *\detailed create by passing a vector of data
    */
  FaceElement(uint_t global_id,
              const std::vector<real_t>& data,
              uint_t pid=0);
  
  /**
    *\detailed create from the given \p point. 
    */     
  FaceElement(const GeomPoint<1>& point, 
              uint_t global_id,
              uint_t pid=0);
  
  
  /**
    * \detailed copy ctor
    */
  FaceElement(const FaceElement& t);
  
  
  /**
    *\detailed copy assignement operator
    */
  FaceElement& operator=(const FaceElement& t);
  
  
  /**
    *\detailed dtor
    */
 virtual ~FaceElement(); //{}
  
  
   /**
     * \detailed add a variable to the array that holds the variables that this object
     * manages
     */
  //void add_variable(const std::string& var_name,uint_t n_components)
  //{dof_object_.add_variable(var_name,n_components);}
  
  
  /**
    * \detailed remove the variable \p var_name
    */
    //void remove_variable(const std::string& var_name){dof_object_.remove_variable(var_name);}
 
    /**
      * \detailed set the global dof index for the variable with name \p var_name for the variable
      * component \p comp_idx
      */
    //void set_comp_var_dof_idx(const std::string& var_name, uint_t comp_idx, uint_t dof_idx)
    //{dof_object_.set_comp_var_dof_idx(var_name,comp_idx,dof_idx);}
    
    
    /**
      * \detailed set the dofs for the variable \p var_name
      */
    //void set_var_dofs(const std::string& var_name,const std::vector<Dof>& dofs)
    //{dof_object_.set_var_dofs(var_name,dofs);}
    
    
    /**
      *\detailed invalidate the dofs for the variable \p var_name
      */
    //void invalidate_dofs(const std::string& var_name){dof_object_.invalidate_dofs(var_name);}
    
    
    /**
      * \detailed invaidate all the dofs for all variables
      */
    //void invalidate_dofs(){dof_object_.invalidate_dofs();}
    
    
    /**
      *\detailed clear the list of variables this object handles
      */
    //void clear_dofs(){dof_object_.clear();}
    
    
    /**
      *\detailed make this node a vertex
      */
    void make_vertex(){is_vertex_ = true;}
    
    
    /**
      *  set the pointer to the
      *  element that holds the side.
      */
   void set_owner_element(Element<1>* o){owner_ = o;}


 /**
   * set the pointer to the element that
   * shares this side. It also sets the old sharing element
   * to the element we use before calling this function
   */
  void set_shared_element(Element<1>* n){neighbor_ = n;}


 /**
   * set the pointers for the shared and owner of this side
   */
  void set_owner_shared_elements(Element<1>* o, Element<1>* n)
  {owner_ = o; neighbor_ = n;}
  
  
  /**
    *\detailed set the boundary indicator for the FaceElement
    */
  void set_boundary_indicator(uint_t bind){boundary_indicator_=bind;}


     /**
       *\detailed get the dof index for the component \p comp_idx for the variable \p var_name
       */
   //uint_t comp_var_dof_index(const std::string& var_name,uint_t comp_idx)const
  //  {return dof_object_.comp_var_dof_index(var_name,comp_idx);}
   
   
    /**
      * \detailed get the Dof vector for the variable \p var_name 
      */
    //std::vector<Dof> var_dof_vector(const std::string& var_name)const{return dof_object_.var_dof_vector(var_name);}
    
    
    /**
      *\detailed returns true iff the object has variables
      */
    //bool has_variables()const{return dof_object_.has_variables();}
    
    
    /**
      *\detailed returns true iff the variable \p var_name is in the list
      * of variables this object manages
      */
    //bool has_variable(const std::string& var_name)const{return dof_object_.has_variable(var_name);}
    
    
    /**
      *\detailed true iff the dof index for the variable \p var_name and its component
      *\p comp is not LibSimPP::invalid_uint()
      */
    //bool has_valid_dof_idx(const std::string& var_name,uint_t comp)const
    //{return dof_object_.has_valid_dof_idx(var_name,comp);}
    
    /**
      *\detailed returns true iff \p make_vertex has been called on this object
      */
    bool is_vertex()const{return is_vertex_;}
    
    
    
    /**
      * @returns \p true iff a boundary indicator has been set
      */
    bool on_boundary () const {return boundary_indicator_!=KernelConsts::invalid_size_type();}
 
 
  /**
    *\detailed get the boundary indicator of the face
    */
   uint_t boundary_indicator()const{return boundary_indicator_;}
   
   /**
    *\detailed get the nodes of the element that are necessary to
    *have an approximation order \p Order
    */
   //virtual void get_order_nodes(NumMethodBase::Order o,
   //                             std::vector<const Node<1>* >& nodes)const=0;
    
    
    /**
     *\detailed print the information for the MeshEntity
     */
    virtual std::ostream& print_mesh_entity_info(std::ostream &out)const;
    
    
    bool is_active()const{return true;}
    uint_t get_id()const{return id_;}
    void set_id(uint_t id){id_ = id;}
    bool has_valid_id()const{return true;}
  
  private:
  
  
  /**
    *\detailed an object that handles the dofs
    *for this FaceElement
    */
 // DofObject dof_object_;
  
  
  /**
   *\detailed the owner of the FaceElement
   */
  Element<1>* owner_;
 
 
 /**
   *\detailed the neighbor of the FaceElement
   */
  Element<1>* neighbor_;
  
  
  /**
    *\detailed flag indicating if the node is a vertex
    *default is false
    */
   bool is_vertex_;
   
    
  /**
    *\detailed the boundary indicator of the Node
    */
  uint_t boundary_indicator_;

  uint_t id_;

};


inline
std::ostream& 
FaceElement<1,0>::print_mesh_entity_info(std::ostream &out)const
{
  /*this->print_point_info(out);
  this->MeshEntity<1,0>::print_mesh_entity_info(out);
  dof_object_.print_dof_object_info(out);*/
  
  return out;

}





/*
   FaceElement<spacedim,1> exists only when MeshGeometry::dim_ = 2 or 3
   These inherit from Element<spacedim,1> as they are elements embedded in
   a space of MeshGeometry::dim_ space. FaceElement<2,1> objects actually bound the
   2D elements we support. They can be inquired about the boundary they may be located
   which element owns them and if they are internal which element shares them.
   In 2D a FaceElement<2,1> is owned/shared by a Face<2> object meaning an Element<2,2>. 
   In 3D a FaceElement<3,1> is owned/shared by a Face<3> object meaning an Element<3,2>. 
   Concrete classes implement the pure virtual functions inherited by Element<spacedim,1>
   Concrete classes of this class are: Edge<2> and Edge<3>.
   
 */



template<>
class FaceElement<2,1>/*: public BaseElement<element_traits<FaceElement<2,1> > >*/
{

 protected:
 
  FaceElement();
  
  FaceElement(uint_t id,uint_t pid=0);
  
 public:
 
 static const int dim_ = 2;
 static const int topodim_ = 1;
 
 typedef EdgeSelector<2>::ptr_t edge_ptr_t;
 typedef EdgeSelector<2>::ptr_t face_ptr_t;
 
 /**
   *\detailed dtor
   */
 virtual ~FaceElement(){}
 

 /**
   *  set the pointer to the
   *  element that holds the side.
   */
 void set_owner_element(Element<2>* o){owner_ = o;}


 /**
   * set the pointer to the element that
   * shares this side. It also sets the old sharing element
   * to the element we use before calling this function
   */
 void set_shared_element(Element<2>* n){neighbor_ = n;}


 /**
   * set the pointers for the shared and owner of this side
   */
 void set_owner_shared_elements(Element<2>* o, Element<2>* n)
 {owner_ = o; neighbor_ = n;}
 
 
 /**
   *\detailed set the boundary indicator for the FaceElement
   */
  void set_boundary_indicator(uint_t bind){boundary_indicator_=bind;}


 /**
   *\detailed read/write access to the owner of this
   * face. May return NULL
   */
 Element<2>* owner(){return owner_;}
 
 
 /**
   *\detailed read/write access to the neighbor of this
   * face may return NULL in this case the face is on the
   * boundary
   */
 Element<2>* neighbor(){return neighbor_;}
 
 
 /**
   * @returns \p true iff a boundary indicator has been set
   */
 bool on_boundary () const {return true;/*boundary_indicator_!=LibSimPP::internal_mesh_entity_id();*/}
 
 
 /**
   *\detailed get the boundary indicator of the face
   */
 uint_t boundary_indicator()const{return boundary_indicator_;}
 
 
 /**
   *\detailed print the information for the MeshEntity
   */
 virtual std::ostream& print_mesh_entity_info(std::ostream &out)const;
 
 
  bool is_active()const{return true;}
  uint_t get_id()const{return id_;}
  void set_id(uint_t id){id_ = id;}
  bool has_valid_id()const{return true;}
 
 private:
 
 
 /**
   *\detailed the owner of the FaceElement
   */
  Element<2>* owner_;
 
 
 /**
   *\detailed the neighbor of the FaceElement
   */
  Element<2>* neighbor_;
  
   
  /**
    *\detailed the boundary indicator of the edge
    *the default is LibSimPP::internal_mesh_entity_id()
    */
  uint_t boundary_indicator_;
  
  /**
    *\detailed we fail to link with the print_mesh_entity_info
    */
  std::ostream& print_(std::ostream& o)const;

  uint_t id_;


};


inline
std::ostream& 
FaceElement<2,1>::print_mesh_entity_info(std::ostream &out)const
{

  return print_(out);
}


//forward declaration 
class Face;

template<>
class FaceElement<3,1>/*: public BaseElement<element_traits<FaceElement<3,1> > >*/
{

 protected:
 

  FaceElement();
  
  
  FaceElement(uint_t id,uint_t pid=0);
  
 public:
 
 static const int dim_ = 3;
 static const int topodim_ = 1;
 
 typedef EdgeSelector<3>::ptr_t   edge_ptr_t;
 typedef EdgeSelector<3>::ptr_t face_ptr_t;
 
 /**
   *\detailed dtor
   */
 virtual ~FaceElement(){}
 
 
 /**
   *  set the pointer to the
   *  element that holds the side.
   */
 void set_owner_element(Face* o){owner_ = o;}


 /**
   * set the pointer to the element that
   * shares this side. It also sets the old sharing element
   * to the element we use before calling this function
   */
 void set_shared_element(Face* n){neighbor_ = n;}


 /**
   * set the pointers for the shared and owner of this side
   */
 void set_owner_shared_elements(Face* o, Face* n)
 {owner_ = o; neighbor_ = n;}
 
 
 /**
   *\detailed set the boundary indicator for the FaceElement
   */
  void set_boundary_indicator(uint_t bind){boundary_indicator_=bind;}

 /**
   *\detailed read/write access to the owner of this
   * face. May return NULL
   */
 Face* owner(){return owner_;}
 
 
 /**
   *\detailed read/write access to the neighbor of this
   * face may return NULL in this case the face is on the
   * boundary
   */
 Face* neighbor(){return neighbor_;}
 
 
 /**
   * @returns \p true iff a boundary indicator has been set
   */
 bool on_boundary () const {return true;/*boundary_indicator_!=LibSimPP::internal_mesh_entity_id();*/}
 
 
  /**
   *\detailed get the boundary indicator of the face
   */
 uint_t boundary_indicator()const{return boundary_indicator_;}
 
 
 /**
   *\detailed print the information for the MeshEntity
   */
  virtual std::ostream& print_mesh_entity_info(std::ostream &out)const;
  
  
   bool is_active()const{return true;}

   uint_t get_id()const{return id_;}
   void set_id(uint_t id){id_ = id;}
   bool has_valid_id()const{return true;}
 
 private:
 
 
 /**
   *\detailed the owner of the FaceElement
   */
  Face* owner_;
 
 
 /**
   *\detailed the neighbor of the FaceElement
   */
  Face* neighbor_;
  
   
  /**
    *\detailed the boundary indicator of the edge
    *the default is LibSimPP::internal_mesh_entity_id()
    */
  uint_t boundary_indicator_;
  
  /**
    *\detailed we fail to link with the print_mesh_entity_info
    */
  std::ostream& print_(std::ostream& o)const;

  uint_t id_;


};

inline
std::ostream& 
FaceElement<3,1>::print_mesh_entity_info(std::ostream &out)const
{
  return print_(out);
}


/**
  * A Face<3> IsA FaceElement<3,2>
  *
  * This is the base class for representing faces in MeshGeometry::dim_ = 3
  * spatial dimension. Concrete classes of this class implement the pure virtual
  * functions inherited by Element<3,2> class. Concrete classes of this class are
  * Quad<3>, Tri<3>.
  */
  
template<>
class FaceElement<3,2>/*: public BaseElement<element_traits<FaceElement<3,2> > >*/
{

 protected:
 

 FaceElement();
 
 FaceElement(uint_t id,uint_t pid=0);

 public:
 
 
 static const int dim_ = 3;
 static const int topodim_ = 2;
 
 typedef EdgeSelector<3>::ptr_t edge_ptr_t;
 typedef EdgeSelector<3>::ptr_t face_ptr_t;
 
 
 /**
   *\detailed dtor
   */
 virtual ~FaceElement(){}
 

 /**
   *  set the pointer to the
   *  element that holds the side.
   */
 void set_owner_element(Element<3>* o){owner_ = o;}


 /**
   * set the pointer to the element that
   * shares this side. It also sets the old sharing element
   * to the element we use before calling this function
   */
 void set_shared_element(Element<3>* n){neighbor_ = n;}


 /**
   * set the pointers for the shared and owner of this side
   */
 void set_owner_shared_element(Element<3>* o, Element<3>* n)
 {owner_ = o; neighbor_ = n;}
 
 
 /**
    *\detailed set the boundary indicator for the FaceElement
    */
  void set_boundary_indicator(uint_t bind){boundary_indicator_=bind;}
  
  


  /**
   *\detailed read/write access to the owner of this
   * face. May return NULL
   */
  Element<3>* owner(){return owner_;}
  
  
   /**
   *\detailed read/write access to the neighbor of this
   * face. May return NULL
   */
  Element<3>* neighbor(){return neighbor_;}


  /**
      * @returns \p true iff a boundary indicator has been set
      */
    bool on_boundary () const { return false;/*return boundary_indicator_!=LibSimPP::internal_mesh_entity_id();*/}
  
  
 /**
   *\detailed get the boundary indicator of the face
   */
 uint_t boundary_indicator()const{return boundary_indicator_;}
  
  
  /**
   *\detailed print the information for the MeshEntity
   */
  virtual std::ostream& print_mesh_entity_info(std::ostream &out)const;
  
  
   bool is_active()const{return true;}

   uint_t get_id()const{return id_;}
   void set_id(uint_t id){id_ = id;}
   bool has_valid_id()const{return true;}

  protected:
  
  
  /**
   *\detailed the owner of the FaceElement
   */
  Element<3>* owner_;
 
 
 /**
   *\detailed the neighbor of the FaceElement
   */
  Element<3>* neighbor_;
  
  
  /**
    *\detailed the boundary indicator of the edge
    */
  uint_t boundary_indicator_;

  uint_t id_;
  
  /**
    *\detailed we fail to link with the print_mesh_entity_info
    */
  std::ostream& print_(std::ostream& o)const;

};

inline
std::ostream& 
FaceElement<3,2>::print_mesh_entity_info(std::ostream &out)const
{
  return print_(out);
}

}

}
#endif
