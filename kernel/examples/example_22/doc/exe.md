# Example 22: Create a graph from a ```Mesh```

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
#include "kernel/data_structs/boost_serial_graph.h"
#include "kernel/data_structs/serial_graph_builder.h"

#include <iostream>
```

## <a name="m_func"></a> The main function

```
namespace example
{
using kernel::real_t;
using kernel::uint_t;
using kernel::Null;
using kernel::numerics::Mesh;
using kernel::GeomPoint;
using kernel::BoostSerialGraph;

}

int main(){

    using namespace example;

    try{

        Mesh<2> mesh;

        {
            uint_t nx = 2;
            uint_t ny = 2;

            GeomPoint<2> start(0.0);
            GeomPoint<2> end(1.0);

            std::cout<<"Starting point: "<<start<<std::endl;
            std::cout<<"Ending point: "<<end<<std::endl;

            // generate the mesh
            kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);

            std::cout<<"MESH STATISTICS "<<std::endl;
            std::cout<<"Number of elements: "<<mesh.n_elements()<<std::endl;
            std::cout<<"Number of nodes: "<<mesh.n_nodes()<<std::endl;
            std::cout<<"Number of faces: "<<mesh.n_faces()<<std::endl;
            std::cout<<"Number of edges: "<<mesh.n_edges()<<std::endl;
            std::cout<<"Number of boundaries: "<<mesh.n_boundaries()<<std::endl;
            std::cout<<"\n";
        }

        // create the graph from the mesh
        BoostSerialGraph<Null, Null> graph;

        // build the mesh graph
        build_mesh_graph(mesh, graph);

        std::cout<<"Number of vertices: "<<graph.n_vertices()<<std::endl;
        std::cout<<"Number of edges: "<<graph.n_edges()<<std::endl;


    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
```

## <a name="results"></a> Results

```
Starting point: ( 0,0 )
Ending point: ( 1,1 )
MESH STATISTICS
Number of elements: 4
Number of nodes: 9
Number of faces: 12
Number of edges: 12
Number of boundaries: 4

Number of vertices: 4
Number of edges: 8
```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




