#include "kernel/base/types.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/element.h"
#include "kernel/utilities/filtered_iterator.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/utilities/vtk_mesh_file_writer.h"
#include "kernel/numerics/scalar_fv_system.h"

#include <iostream>

namespace example
{
using kernel::real_t;
using kernel::uint_t;
using kernel::numerics::Mesh;
using kernel::GeomPoint;
using kernel::numerics::ScalarFVSystem;
}


int main(){

    using namespace example;

    try{

        Mesh<2> mesh;

        {
            // generate the mesh

            uint_t nx = 10;
            uint_t ny = 10;

            GeomPoint<2> start(0.0);
            GeomPoint<2> end(1.0);

            kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);
        }

        // laplace system
        //ScalarFVSystem<2> laplace("Laplace", "U", mesh);

        // distribute the dofs
        //laplace.distribute_dofs();

        //std::cout<<"Number of dofs: "<<laplace.n_dofs()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }









    return 0;
}




