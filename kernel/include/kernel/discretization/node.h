#ifndef NODE_H
#define NODE_H

#include "kernel/base/types.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/numerics/dof_object.h"
#include "kernel/discretization/face_element.h"
#include "kernel/base/kernel_consts.h"

#include <array>

namespace kernel
{

namespace numerics
{

/// \brief Wraps the notion of a node. A node
/// is simply a point in dim-space that can hold
/// dofs
template<int dim>
class Node: public FaceElement<dim, 0>
{

public:

    /// \brief Constructor
    Node();

    /// \brief Constructor
    Node(real_t coord, uint_t id);

    /// \brief Constructor
    Node(const std::array<real_t, dim>& coords, uint_t id);

    /// \brief Constructor
    Node(const GeomPoint<dim>& point, uint_t global_id, uint_t pid);

    /// \brief Returns true is the Node is a vertex
    bool is_vertex()const{return is_vertex_;}

    /// \brief Signal the Node that it is a vertex
    void make_vertex(){is_vertex_ = true;}

private:

    /// \brief is the Node a vertex
    bool is_vertex_;

};

/*
template<>
class Node<1>: public FaceElement<1,0>

{


  public:


  Node();


  explicit Node(uint_t global_id,
                real_t val=0.0,
                uint_t pid=0);


  Node(uint_t global_id,
       const std::vector<real_t>& data,
       uint_t pid=0);


  Node(const GeomPoint<1>& point,
       uint_t global_id,
       uint_t pid=0);



  Node(const Node& t);



  Node& operator=(const Node& t);



  ~Node(){}



  std::ostream& print_node_info(std::ostream &out)const;

};

inline
Node<1>::Node()
               :
               FaceElement<1,0>()
               {}

inline
Node<1>::Node(uint_t global_id,
                     real_t val,
                     uint_t pid)
               :
               FaceElement<1,0>(global_id,val,pid)
               {}

inline
Node<1>::Node(uint_t global_id,
              const std::vector<real_t>& data,
              uint_t pid)
                :
              FaceElement<1,0>(global_id,data,pid)
                {}


inline
Node<1>::Node(const GeomPoint<1>& point,
                     uint_t global_id,
                     uint_t pid)
                     :
                     FaceElement<1,0>(point,global_id,pid)
                     {}

inline
Node<1>::Node(const Node<1>& t)
               :
               FaceElement<1,0>(t)
               {}

inline
Node<1>&
Node<1>::operator=(const Node<1>& o)
{

  if(this==&o)return *this;

  this->FaceElement<1,0>::operator=(o);
  return *this;
}

inline
std::ostream&
Node<1>::print_node_info(std::ostream &out)const
{

  this->print_mesh_entity_info(out);
  return out;
}

*/

}

}

#endif // NODE_H
