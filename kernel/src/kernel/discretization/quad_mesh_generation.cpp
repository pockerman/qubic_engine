#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/discretization/mesh.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/mesh_connectivity.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/node.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/element_type.h"
#include "kernel/base/kernel_consts.h"

#include <exception>

namespace kernel
{
namespace numerics
{

namespace
{
      inline
      uint_t idx(uint_t nx,
                    uint_t i,
                    uint_t j)
      {
             return i+j*(nx+1);
      }

 void build_rectangular_mesh_vertices(const GeomPoint<2>& lower_point,
                                       const GeomPoint<2>& upper_point,
                                       uint_t Nx,uint_t Ny, Mesh<2>& mesh)
 {


   const GeomPoint<2>& p1 = lower_point;
   const GeomPoint<2>& p2 = upper_point;

   const real_t dx = (p2[0]-p1[0])/static_cast<real_t>(Nx);
   const real_t dy = (p2[1]-p1[1])/static_cast<real_t>(Ny);

   //dummy variable for counting the vertices in the mesh
   uint_t node_id = 0;

   //construct the vertices of the mesh
   for(uint_t j=0; j<=Ny; ++j)
   {

     for(uint_t i=0; i<=Nx; ++i)
     {
       real_t data[] = {p1[0]+i*dx, p1[1]+j*dy};
       mesh.create_vertex(GeomPoint<2>(data), node_id++);
     }
   }

 }

 void build_rectangular_mesh_elements(uint_t Nx, uint_t Ny, uint_t n_nodes_per_elem,
                                       Mesh<2>& mesh, std::vector<MeshConnectivity>& vertex_connection)
  {


    MeshConnectivity element_to_node_connection(n_nodes_per_elem);

    //dummy variable for the id of the element
    uint_t elem_id = 0;

    //build the elements
     for (uint_t j=0; j<Ny; j++)
     {
      for(uint_t i = 0; i<Nx; i++)
       {

          //build the element
          auto* element = mesh.create_element(ElementType::sub_type::QUAD4, elem_id++, 0);

          if(!element){
              throw std::logic_error("NULL pointer element");
          }

          element->reserve_nodes(4);

          //uint_t ele_id = element->get_id();

          uint_t node_id = idx(Nx, i, j);

          auto* node = mesh.node(node_id);
          element->append_node(node);

          //element_to_node_connection.set_connection(0,node_id);
          //vertex_connection[node_id].push_back(ele_id);

          node_id = idx(Nx,i+1,j);
          node = mesh.node(node_id);
          element->append_node(node);
          //element_to_node_connection.set_connection(1,node_id);
          //vertex_connection[node_id].push_back(ele_id);

          node_id = idx(Nx,i+1,j+1);
          node = mesh.node(node_id);
          element->append_node(node);
          //element_to_node_connection.set_connection(2,node_id);
          //vertex_connection[node_id].push_back(ele_id);

          node_id = idx(Nx,i,j+1);
          node = mesh.node(node_id);
          element->append_node(node);
          //element_to_node_connection.set_connection(3,node_id);
          //vertex_connection[node_id].push_back(ele_id);

         //set the vertices connectivity
         //element->get_node_connectivity() = element_to_node_connection;
       }
     }
  }

 void build_quad_mesh_element_element_connectivity(Mesh<2>& m,
                                                  const std::vector<MeshConnectivity>& vertices_connections){


    //typedef Mesh<2>::element_iterator<ActiveElement<2> > ElementIterator;
    ElementMeshIterator<Active, Mesh<2>> iterator(m);

    auto e_it = iterator.begin();
    auto e_it_end = iterator.end();

    for(; e_it != e_it_end; ++e_it)
    {

      Element<2>* element = *e_it; //.get_iterator();

      //here are the vertices of the element
      //const MeshConnectivity& ele_node_connections = element->get_node_connectivity();

      //the neighbors of the current element
      MeshConnectivity neigh_connections(element->n_faces());

      //loop over the faces of the element
      for(uint_t face=0; face<element->n_faces(); ++face)
      {

         //uint_t neigh_id = process_quad_element(element,face,vertices_connections);
         //neigh_connections.set_connection(face,neigh_id);
      }

      //set the neighbor indices for the element
      //element->get_neighbor_connectivity() = neigh_connections;
    }
}


 void build_quad_mesh_element_face_connectivity(Mesh<2>& m){

  ElementMeshIterator<Active, Mesh<2>> iterator(m);

  auto e_it = iterator.begin();
  auto e_it_end = iterator.end();

  uint_t face_counter = 0;

  for(; e_it!=e_it_end; ++e_it)
  {

    Element<2>* element = *e_it; //.get_iterator();

    //get the neighbor connectivity
    //const MeshConnectivity& neigh_connections = element->get_neighbor_connectivity();

    //AssertBiblSimPP(neigh_connections.size()==4,ExecSizeMismatch(neigh_connections.size(),4));

    //MeshConnectivity& face_connections = element->get_face_connectivity();

    /*if(face_connections.empty())
    face_connections.set_connectivity_size(4);


    for(uint_t neigh=0; neigh<neigh_connections.size(); ++neigh){

       if(neigh_connections[neigh] == KernelConsts::invalid_size_type())
       {

         //this is a boundary face
         FaceElement<2,1>* face = m.create_face(ElementType::sub_type::EDGE2,face_counter++);

         //AssertBiblSimPP(face!=nullptr,ExecNullPointer());

         //by default all boundary faces get an indicator zero
         face->set_boundary_indicator(0);

         face->set_owner_element(element);

         std::vector<uint_t> face_vertices;

         //get the vertices of the element at the given face
         element->face_vertices(neigh,face_vertices);

         //AssertBiblSimPP(face_vertices.empty()==false,ExecUninitialized("Empty Face Vertices List"));
         //AssertBiblSimPP(face_vertices.size()==face->n_vertices(),
         //               ExecSizeMismatch(face_vertices.size(),face->n_vertices()));

         face->get_node_connectivity().assign_connections(face_vertices);

         face_connections.set_connection(neigh, face->get_id());

       }
       else
       {
           //this is an internal face. Create the face
           //only if our id is smaller than our neighbor
           if(element->id()<neigh_connections[neigh])
           {

              FaceElement<2,1>* face = m.create_face(ElementType::sub_type::EDGE2,face_counter++);

              //AssertBiblSimPP(face!=nullptr,ExecNullPointer());

              face_connections.set_connection(neigh, face->get_id());


              std::vector<uint_t> face_vertices;

             //get the vertices of the element at the given face
             element->face_vertices(neigh,face_vertices);

             //AssertBiblSimPP(face_vertices.empty()==false,ExecUninitialized("Empty Face Vertices List"));
             //AssertBiblSimPP(face_vertices.size()==face->n_vertices(),
             //               ExecSizeMismatch(face_vertices.size(),face->n_vertices()));

              face->get_node_connectivity().assign_connections(face_vertices);

              Element<2>* neighbor = m.element(neigh_connections[neigh]);

              face->set_owner_element(element);
              face->set_shared_element(neighbor);

              MeshConnectivity& neigh_face_connections = neighbor->get_face_connectivity();

              if(neigh_face_connections.empty())
               neigh_face_connections.set_connectivity_size(4);


              //find out which local neighbor the current element is
              //to our neighbor

              uint_t neigh_idx = neighbor->which_neighbor_am_i(element->id());

              //AssertBiblSimPP(neigh_idx<4,ExecInvalidIndex(neigh_idx,0,4));

              neigh_face_connections.set_connection(neigh_idx,face->id());
           }
       }
    }*/
}

}

}
void build_quad_mesh(Mesh<2>& mesh, uint_t nx, uint_t ny,
                     const GeomPoint<2, real_t>& lower_point,
                     const GeomPoint<2, real_t>& upper_point){


    const uint_t Nx = nx;
    const uint_t Ny = ny;

    //const GeomPoint<2, real_t>& p1 = lower_point;
    //const GeomPoint<2, real_t>& p2 = upper_point;

    //const real_t dx = (p2[0]-p1[0])/static_cast<real_t>(Nx);
    //const real_t dy = (p2[1]-p1[1])/static_cast<real_t>(Nx);


    mesh.reserve_n_nodes((Nx+1)*(Ny+1));

    //build the vertices of the mesh
    build_rectangular_mesh_vertices(lower_point, upper_point,Nx, Ny, mesh);

    //now that we have the vertices lets build the elements
    //reserve space for the elements
    mesh.reserve_n_elements(Nx*Ny);

    std::vector<MeshConnectivity> vertices_connections(mesh.n_vertices(), MeshConnectivity());

    //TODO: change hard-coded for QUAD4 here!!!
    build_rectangular_mesh_elements(Nx,Ny, 4, mesh, vertices_connections);

    //now we build the neighbors of the mesh
    build_quad_mesh_element_element_connectivity(mesh, vertices_connections);


    //count how many faces the mesh has
    uint_t n_internal_faces=0;
    uint_t n_boundary_faces=0;


    ElementMeshIterator<Active, Mesh<2>> iterator(mesh);

    auto e_it = iterator.begin();
    auto e_it_end = iterator.end();

    for(; e_it!=e_it_end; ++e_it)
    {

       const Element<2>* element = *e_it; //.get_iterator();

       //get the neighbor connectivity for the elment
       //const MeshConnectivity& neigh_connections = element->get_neighbor_connectivity();

       //AssertBiblSimPP(neigh_connections.size()==4,ExecSizeMismatch(neigh_connections.size(),4));

       /*for(uint_t n=0; n<neigh_connections.size(); ++n)
       {

           if(neigh_connections[n] == KernelConsts::invalid_size_type())
            n_boundary_faces++;
           else
            n_internal_faces++;
       }*/
    }


    mesh.reserve_n_faces(n_boundary_faces + n_internal_faces/2);
    build_quad_mesh_element_face_connectivity(mesh);

    //finally set the number of boundaries
    mesh.set_n_boundaries(4);

    //loop over the faces and assign boundary indicators
    /*typedef Mesh<2,serial_mode>::face_iterator<ActiveBoundaryFace<2> > FaceIterator;
    typedef Mesh<2,serial_mode>::face_ptr FacePtr;

    FaceIterator faces_begin = mesh.active_boundary_faces_begin();
    FaceIterator faces_end   = mesh.active_boundary_faces_end();


    for(; faces_begin != faces_end; ++faces_begin)
    {

       FacePtr face = *faces_begin.get_iterator();

       const GeomPoint<2> centroid = face->centroid();

       if(centroid[1] == lower_point[1])
       {

          face->set_boundary_indicator(0);
       }
      else if(centroid[0] == upper_point[0])
       {

         face->set_boundary_indicator(1);
       }
     else if(centroid[1]==upper_point[1])
      {

         face->set_boundary_indicator(2);

      }
     else if(centroid[0] == lower_point[0])
      {

        face->set_boundary_indicator(3);
      }
    }*/

}

}

}
