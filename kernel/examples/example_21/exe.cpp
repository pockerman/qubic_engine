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

#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#include "kernel/maths/krylov_solvers/trilinos_krylov_solver.h"
#include "kernel/maths/krylov_solvers/krylov_solver_data.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"
#include "kernel/maths/functions/numeric_scalar_function.h"
#include "kernel/maths/functions/numeric_vector_function.h"

#include "kernel/numerics/scalar_dirichlet_bc_function.h"
#include "kernel/numerics/fv_interpolation_factory.h"
#include "kernel/numerics/fv_interpolation_types.h"
#include "kernel/numerics/scalar_fv_system.h"
#include "kernel/numerics/trilinos_solution_policy.h"
#include "kernel/numerics/fv_convection_assemble_policy.h"
#include "kernel/numerics/boundary_function_base.h"
#include "kernel/numerics/fv_ud_interpolation.h"

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
using kernel::numerics::FVConvectionAssemblyPolicy;
using kernel::numerics::FVInterpolationFactory;
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
    return 0.0;
}


class VelocityVals: public kernel::numerics::NumericVectorFunctionBase<2>
{
public:

    /// \brief Returns the value of the function
    virtual kernel::DynVec<real_t> value(const GeomPoint<2>&  input)const override final;
};

kernel::DynVec<real_t>
VelocityVals::value(const GeomPoint<2>&  /*input*/)const{

   return kernel::DynVec<real_t>(2, 1.0);

}

// The BC description
class AdvectionBC: public kernel::numerics::BoundaryFunctionBase<2>
{
public:

    typedef kernel::numerics::BoundaryFunctionBase<2>::input_t input_t;
    typedef kernel::numerics::BoundaryFunctionBase<2>::output_t output_t;

    // constructor
    AdvectionBC();

    //
    // \brief Returns the value of the function on the Dirichlet boundary
    virtual output_t value(const GeomPoint<2>&  /*input*/)const override{return 1.0;}

};

AdvectionBC::AdvectionBC()
    :
    kernel::numerics::BoundaryFunctionBase<2>()
{
    this->set_bc_type(0, kernel::numerics::BCType::ZERO_DIRICHLET);
    this->set_bc_type(1, kernel::numerics::BCType::ZERO_NEUMANN);
    this->set_bc_type(2, kernel::numerics::BCType::ZERO_NEUMANN);
    this->set_bc_type(3, kernel::numerics::BCType::DIRICHLET);
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

            // generate the mesh
            kernel::numerics::build_quad_mesh(mesh, nx, ny, start, end);
        }

        // data for the solver
        KrylovSolverData data;
        data.tolerance = kernel::KernelConsts::tolerance();
        data.n_iterations = 1000;
        data.solver_type = kernel::numerics::KrylovSolverType::GMRES;
        data.precondioner_type = kernel::numerics::PreconditionerType::ILU;

        // description of the BC the system is using
        AdvectionBC bc_func;

        // description of the RHS
        RhsVals rhs;

        // description of the Velocity field
        VelocityVals velocity;

        // laplace system
        ScalarFVSystem<2, FVConvectionAssemblyPolicy<2>, TrilinosSolutionPolicy> convection("Convection", "U", mesh);

        // system configuration
        convection.set_boundary_function(bc_func);
        convection.set_rhs_function(rhs);
        convection.set_solver_data(data);

        auto interpolate_builder = [](){
            return kernel::numerics::FVInterpolationFactory<2>::build(kernel::numerics::FVInterpolationType::UD);
        };

        convection.get_assembly_policy().build_interpolate_scheme(interpolate_builder);

        std::shared_ptr<kernel::numerics::FVInterpolateBase<2>> interpolation = convection.get_assembly_policy().get_interpolation();
        dynamic_cast<kernel::numerics::FVUDInterpolate<2>*>(interpolation.get())->set_velocity(velocity);


        // distribute the dofs
        convection.distribute_dofs();
        std::cout<<"Number of dofs: "<<convection.n_dofs()<<std::endl;

        convection.assemble_system();
        convection.solve();
        convection.save_solution("convection_system.vtk");

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



