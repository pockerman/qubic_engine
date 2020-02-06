#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/element.h"
#include "kernel/utilities/filtered_iterator.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/numerics/scalar_fv_system.h"
#include "kernel/numerics/trilinos_solution_policy.h"
#include "kernel/numerics/fv_laplace_assemble_policy.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#include "kernel/maths/krylov_solvers/trilinos_krylov_solver.h"

#include <iostream>

namespace example
{
using kernel::real_t;
using kernel::uint_t;
using kernel::numerics::Mesh;
using kernel::GeomPoint;
using kernel::numerics::ScalarFVSystem;
using kernel::numerics::TrilinosSolutionPolicy;
using kernel::numerics::FVLaplaceAssemblyPolicy;
}

int main(){

    using namespace example;

    try{

        Mesh<2> mesh;

        {
            uint_t nx = 10;
            uint_t ny = 10;

            GeomPoint<2> start(0.0);
            GeomPoint<2> end(1.0);

            // generate the mesh
            kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);
        }

        // laplace system
        ScalarFVSystem<2, FVLaplaceAssemblyPolicy<2>, TrilinosSolutionPolicy> laplace("Laplace", "U", mesh);

        // distribute the dofs
        laplace.distribute_dofs();
        std::cout<<"Number of dofs: "<<laplace.n_dofs()<<std::endl;

        laplace.assemble_system();
        laplace.solve();


    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

#else

#include <iostream>
int main(){
    std::cout<<"This example requires Trilinos. Reconfigure kernellib such that it uses Trilinos"<<std::endl;
    return 0;
}
#endif



