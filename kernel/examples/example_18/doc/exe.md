# Example 18: Create A Rectangular Grid

## Contents

* [Overview](#overview) 
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="overview"></a> Overview

## <a name="include_files"></a> Include files

```
#include "kernel/base/types.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/element.h"
#include "kernel/utilities/filtered_iterator.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/utilities/vtk_mesh_file_writer.h"

#include <iostream>
```

## <a name="m_func"></a> The main function

```
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

    // loop over the elements
    auto elem_itr = mesh.elements_begin();
    auto elem_itr_e = mesh.elements_end();

    for(; elem_itr != elem_itr_e; ++elem_itr){

        auto elem = *elem_itr;

        if(elem->is_active()){

            if(elem->get_id() == 50){

                std::cout<<"Number of nodes: "<<elem->n_nodes()<<std::endl;
                std::cout<<"Number of vertices: "<<elem->n_vertices()<<std::endl;
            }
            else{
                elem->set_active_flag(false);
            }
        }
    }

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


    // finally let's save the mesh for visualization
    kernel::numerics::VtkMeshFileWriter mesh_writer("example_18_mesh.vtk", true);
    mesh_writer.write_mesh(mesh);
    return 0;
}
```

## <a name="results"></a> Results

```
MESH STATISTICS
Number of elements: 100
Number of nodes: 121
Number of faces: 0
Number of edges: 0
Number of nodes: 4
Number of vertices: 4
Element id: 50
```

<img src="mesh_view.png"
     alt="Mesh view"
     style="float: left; margin-right: 10px;" />


## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




