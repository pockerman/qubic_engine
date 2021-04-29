#include "kernel/base/types.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face_element.h"
#include "kernel/utilities/filtered_iterator.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/face_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/utilities/predicates.h"
#include "kernel/discretization/vtk_mesh_file_writer.h"

#include <iostream>

namespace example
{
using kernel::real_t;
using kernel::uint_t;
using kernel::numerics::Mesh;
using kernel::GeomPoint;
}


int main(){

    using namespace example;

    Mesh<2> mesh;

    uint_t nx = 10;
    uint_t ny = 10;

    GeomPoint<2> start(0.0);
    GeomPoint<2> end(1.0);

    kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);

    // print some info
    std::cout<<"MESH STATISTICS "<<std::endl;
    std::cout<<"Number of elements: "<<mesh.n_elements()<<std::endl;
    std::cout<<"Number of nodes: "<<mesh.n_nodes()<<std::endl;
    std::cout<<"Number of faces: "<<mesh.n_faces()<<std::endl;
    std::cout<<"Number of edges: "<<mesh.n_edges()<<std::endl;
    std::cout<<"Number of boundaries: "<<mesh.n_boundaries()<<std::endl;
    std::cout<<"\n";

    // loop over the elements
    auto elem_itr = mesh.elements_begin();
    auto elem_itr_e = mesh.elements_end();

    for(; elem_itr != elem_itr_e; ++elem_itr){

        auto elem = *elem_itr;

        if(elem->is_active()){

            if(elem->get_id() == 50){

                std::cout<<"Number of nodes: "<<elem->n_nodes()<<std::endl;
                std::cout<<"Number of vertices: "<<elem->n_vertices()<<std::endl;
                std::cout<<"Number of edges: "<<elem->n_edges()<<std::endl;
                std::cout<<"Number of faces: "<<elem->n_faces()<<std::endl;
            }
            else{
                elem->set_active_flag(false);
            }
        }
    }

    std::cout<<"\n";

    // filtered iteration can also be used
    kernel::numerics::ElementMeshIterator<kernel::numerics::Active, Mesh<2>> filter(mesh);

    auto active_elem_begin = filter.begin();
    auto active_elem_end = filter.end();

    for(; active_elem_begin != active_elem_end; ++active_elem_begin){

        auto elem = *active_elem_begin;
        std::cout<<"Element id: "<<elem->get_id()<<std::endl;
    }

    // activate the elements in order to plot the mesh
    // loop over the elements
    elem_itr = mesh.elements_begin();

    for(; elem_itr != elem_itr_e; ++elem_itr){
        auto elem = *elem_itr;
        elem->set_active_flag(true);
    }

    // loop over the boundary faces and print their ids
    // and the boundary indicator
    kernel::numerics::ConstFaceMeshIterator<kernel::ActiveBoundaryObject, kernel::numerics::Mesh<2>> face_filter(mesh);

    auto face_begin = face_filter.begin();
    auto face_end   = face_filter.end();

    for(; face_begin != face_end; ++face_begin){

        auto* face = *face_begin;
        std::cout<<"face: "<<face->get_id()<<" is at boundary: "<<face->boundary_indicator()<<std::endl;
    }

    // finally let's save the mesh for visualization
    kernel::numerics::VtkMeshFileWriter mesh_writer("example_18_mesh.vtk", true);
    mesh_writer.write_mesh(mesh);
    return 0;
}




