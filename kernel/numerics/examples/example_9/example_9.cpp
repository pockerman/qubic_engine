#include "kernel/base/config.h"

#if defined(USE_TRILINOS) && defined(USE_FVM)

#include "kernel/base/types.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/quad_mesh_generation.h"
#include "kernel/geometry/geom_point.h"
#include "kernel/discretization/element.h"
#include "kernel/utilities/filtered_iterator.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/numerics/pdes/fv_scalar_system.h"
#include "kernel/numerics/fvm/fv_laplace_assemble_policy.h"
#include "kernel/numerics/trilinos_solution_policy.h"
#include "kernel/numerics/fvm/fv_grad_factory.h"
#include "kernel/numerics/fvm/fv_grad_types.h"
#include "kernel/numerics/scalar_dirichlet_bc_function.h"
#include "kernel/numerics/boundary_function_base.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"

#include "kernel/numerics/krylov_solvers/trilinos_krylov_solver.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_data.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_type.h"
#include "kernel/numerics/krylov_solvers/preconditioner_type.h"
#include "kernel/maths/functions/numeric_scalar_function.h"


#include <cmath>
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
using kernel::numerics::ScalarDirichletBCFunc;
using kernel::numerics::KrylovSolverData;

class RhsVals: public kernel::numerics::NumericScalarFunction<2>
{
public:

    /// \brief Returns the value of the function
    virtual real_t value(const GeomPoint<2>&  input)const override final;
};

real_t
RhsVals::value(const GeomPoint<2>& /*point*/)const{
    return 1.0;
}

}

int main(){

    using namespace example;

    try{

        Mesh<2> mesh;

        {
            uint_t nx = 100;
            uint_t ny = 100;

            GeomPoint<2> start(0.0);
            GeomPoint<2> end(1.0);

            std::cout<<"Starting point: "<<start<<std::endl;
            std::cout<<"Ending point: "<<end<<std::endl;

            // generate the mesh
            kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);
        }

        // data for the solver
        KrylovSolverData data;
        data.tolerance = kernel::KernelConsts::tolerance();
        data.n_iterations = 1000;
        data.solver_type = kernel::numerics::KrylovSolverType::CG;
        data.precondioner_type = kernel::numerics::PreconditionerType::ILU;

        // description of the BC the system is using
        ScalarDirichletBCFunc<2> bc_func(0.0, mesh.n_boundaries());

        RhsVals rhs;

        // laplace system
        ScalarFVSystem<2, FVLaplaceAssemblyPolicy<2>, TrilinosSolutionPolicy> laplace("Laplace", "U", mesh);

        // system configuration
        laplace.set_boundary_function(bc_func);
        laplace.set_rhs_function(rhs);
        laplace.set_solver_data(data);

        auto grad_builder = [](){
            return kernel::numerics::FVGradFactory<2>::build(kernel::numerics::FVGradType::GAUSS);
        };

        laplace.get_assembly_policy().build_gradient(grad_builder);

        // distribute the dofs
        laplace.distribute_dofs();
        std::cout<<"Number of dofs: "<<laplace.n_dofs()<<std::endl;

        laplace.assemble_system();
        laplace.solve();
        laplace.save_solution("laplace_system.vtk");

    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
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



