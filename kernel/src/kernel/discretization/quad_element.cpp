#include "kernel/discretization/quad_element.h"
#include "kernel/discretization/node.h"
#include "kernel/geometry/geometry_utils.h"

#include <cmath>
#include <exception>
namespace kernel
{
namespace numerics
{

namespace  {

void do_get_face_vertices(uint_t f, const std::vector<Node<2>*>& nodes, std::vector<uint_t>& ids){

    if(ids.size() != 2){
        ids.clear();
        ids.resize(2);
    }

    switch(f){
    case 0:
            ids[0] = nodes[0]->get_id();
            ids[1] = nodes[1]->get_id();
        break;
    case 1:
            ids[0] = nodes[1]->get_id();
            ids[1] = nodes[2]->get_id();
    break;
    case 2:
            ids[0] = nodes[2]->get_id();
            ids[1] = nodes[3]->get_id();
    break;
    case 3:
            ids[0] = nodes[3]->get_id();
            ids[1] = nodes[0]->get_id();
    break;
    }
}

}


Quad<2>::Quad(uint_t n)
               :
               Element<2>(),
               impl_(n)
               {}

Quad<2>::Quad(uint_t id, uint_t pid, uint_t n)
               :
               Element<2>(id,pid),
               impl_(n)
               {}

void
Quad<2>::resize_faces(){
    faces_.resize(n_faces(), nullptr);
}

void
Quad<2>::set_face(uint_t f, Quad<2>::face_ptr_t face){

    if(f >= n_faces()){
        throw std::logic_error("Invalid node index: " +
                               std::to_string(f) +
                               " not in [0," +
                               std::to_string(n_faces()) +
                               ")");
    }

    faces_[f] = face;
}

Quad<2>::cface_ref_t
Quad<2>::get_face(uint_t f)const{

    if(f >= n_faces()){
        throw std::logic_error("Invalid face index: " +
                               std::to_string(f) +
                               " not in [0," +
                               std::to_string(n_faces()) +
                               ")");
    }

    if(faces_.empty()){
        throw std::logic_error("Faces have not been initialized for element: " +
                               std::to_string(this->get_id()));
    }

    if(faces_[f] == nullptr){
        throw std::logic_error("Face requested is NULL");
    }

    return *faces_[f];
}


Quad<2>::face_ref_t
Quad<2>::get_face(uint_t f){

    if(f >= n_faces()){
        throw std::logic_error("Invalid node index: " +
                               std::to_string(f) +
                               " not in [0," +
                               std::to_string(n_faces()) +
                               ")");
    }

    if(faces_.empty()){
        throw std::logic_error("Faces have not been initialized for element: " +
                               std::to_string(this->get_id()));
    }

    if(faces_[f] == nullptr){
        throw std::logic_error("Face requested is NULL");
    }

    return *faces_[f];
}

void
Quad<2>::face_vertices(uint_t f, std::vector<uint_t>& ids)const{

    if(f >= n_faces()){
        throw std::logic_error("Invalid node index: " +
                               std::to_string(f) +
                               " not in [0," +
                               std::to_string(n_faces()) +
                               ")");
    }

    if(ids.size() != 2){
        ids.clear();
        ids.resize(2);
    }

    // prefer going via the faces list
    // but sometime this may not be available
    if(this->faces_.empty()){
        do_get_face_vertices(f, this->nodes_, ids);
    }
    else{

        if(this->faces_[f] == nullptr){
           do_get_face_vertices(f, this->nodes_, ids);
        }
        else{
            ids = this->faces_[f]->get_vertices_ids();
        }
    }

}

real_t
Quad<2>::volume()const{

    //        3           2      D           C
      // QUAD4: o-----------o	 o-----------o
      //        |           |	 |           |
      //        |           |	 |           |
      //        |           | 	 |           |
      //        |           |	 |           |
      //        |           |	 |           |
      //        o-----------o	 o-----------o
      //        0           1	 A           B


     // Vector pointing from A to C
     GeomPoint<2> AC ( *this->nodes_[2] - *this->nodes_[0]);

     // Vector pointing from A to B
     GeomPoint<2> AB ( *this->nodes_[1] - *this->nodes_[0]);

     // Vector pointing from A to D
     GeomPoint<2> AD ( *this->nodes_[3] - *this->nodes_[0]);

     // The diagonal vector minus the side vectors
     GeomPoint<2> AC_AB_AD (AC - AB - AD);

     // Check for quick return for planar QUAD4.  This will
     // be the most common case, occuring for all purely 2D meshes.
     if (AC_AB_AD == GeomPoint<2>(0.0)){
       return cross_product(AB, AD).L2_norm();
     }
     else
     {
         // Use 2x2 quadrature to approximate the surface area.  (The
         // true integral is too difficult to compute analytically.)  The
         // accuracy here is exactly the same as would be obtained via a
         // call to Elem::volume(), however it is a bit more optimized to
         // do it this way.  The technique used is to integrate the magnitude
         // of the normal vector over the whole area.  See for example,
         //
         // Y. Zhang, C. Bajaj, G. Xu. Surface Smoothing and Quality
         // Improvement of Quadrilateral/Hexahedral Meshes with Geometric
         // Flow. The special issue of the Journal Communications in
         // Numerical Methods in Engineering (CNME), submitted as an
         // invited paper, 2006.
         // http://www.ices.utexas.edu/~jessica/paper/quadhexgf/quadhex_geomflow_CNM.pdf

         // 4-point rule
         const static real_t q[2] = {0.5 - std::sqrt(3.) / 6., 0.5 + std::sqrt(3.) / 6.};

         real_t vol=0.;
         for (uint_t i=0; i<2; ++i)
           for (uint_t j=0; j<2; ++j)
               vol += cross_product( (AB + q[i]*AC_AB_AD), (AD + q[j]*AC_AB_AD)).L2_norm();

         return 0.25*vol;
     }
}

}
}
