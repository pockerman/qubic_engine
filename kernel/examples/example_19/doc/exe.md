# Example 19: Solve Laplace's Equation With FVM

## Contents

* [Overview](#overview) 
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="overview"></a> Overview

## <a name="include_files"></a> Include files

```
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
#include "kernel/numerics/scalar_dirichlet_bc_function.h"
#include "kernel/maths/krylov_solvers/krylov_solver_data.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"
#include "kernel/numerics/fv_grad_factory.h"
#include "kernel/numerics/fv_grad_types.h"
#include "kernel/maths/functions/numeric_scalar_function.h"
#include "kernel/numerics/boundary_function_base.h"

#include <cmath>
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
```

## <a name="results"></a> Results

```
Starting point: ( 0,0 )
Ending point: ( 1,1 )
Number of dofs: 10000

                *******************************************************
                ***** Problem: Epetra::CrsMatrix
                ***** Preconditioned CG solution
                ***** ILU(0) domain decomp. without overlap
                ***** No scaling
                ***** NOTE: convergence VARIES when the total number of
                *****       processors is changed.
                *******************************************************

AZ_check_options: WARNING: Preconditioned matrix may not be symmetric.


*********************************************************************
*****  Condition number estimate for subdomain preconditioner on PE 0 = 1.7071e+00
*********************************************************************
                iter:    0           residual = 1.000000e+00
                iter:    1           residual = 4.800774e+00
                iter:    2           residual = 4.300148e+00
                iter:    3           residual = 3.839278e+00
                iter:    4           residual = 3.543250e+00
                iter:    5           residual = 3.376338e+00
                iter:    6           residual = 3.267088e+00
                iter:    7           residual = 3.078987e+00
                iter:    8           residual = 2.773267e+00
                iter:    9           residual = 2.451940e+00
                iter:   10           residual = 2.186527e+00
                iter:   11           residual = 1.964334e+00
                iter:   12           residual = 1.747849e+00
                iter:   13           residual = 1.524745e+00
                iter:   14           residual = 1.306708e+00
                iter:   15           residual = 1.101180e+00
                iter:   16           residual = 9.052410e-01
                iter:   17           residual = 7.174600e-01
                iter:   18           residual = 5.381350e-01
                iter:   19           residual = 3.705766e-01
                iter:   20           residual = 2.300593e-01
                iter:   21           residual = 1.347870e-01
                iter:   22           residual = 9.050665e-02
                iter:   23           residual = 8.960784e-02
                iter:   24           residual = 9.210915e-02
                iter:   25           residual = 5.514978e-02
                iter:   26           residual = 3.161312e-02
                iter:   27           residual = 3.110760e-02
                iter:   28           residual = 2.241342e-02
                iter:   29           residual = 1.380230e-02
                iter:   30           residual = 1.348746e-02
                iter:   31           residual = 7.879729e-03
                iter:   32           residual = 5.985208e-03
                iter:   33           residual = 4.102000e-03
                iter:   34           residual = 2.476748e-03
                iter:   35           residual = 1.805104e-03
                iter:   36           residual = 1.047876e-03
                iter:   37           residual = 7.969008e-04
                iter:   38           residual = 5.143099e-04
                iter:   39           residual = 5.088492e-04
                iter:   40           residual = 5.269661e-04
                iter:   41           residual = 6.419753e-04
                iter:   42           residual = 6.016462e-04
                iter:   43           residual = 4.251268e-04
                iter:   44           residual = 2.779335e-04
                iter:   45           residual = 1.557795e-04
                iter:   46           residual = 1.029964e-04
                iter:   47           residual = 5.524558e-05
                iter:   48           residual = 3.245886e-05
                iter:   49           residual = 2.130091e-05
                iter:   50           residual = 1.669330e-05
                iter:   51           residual = 1.300442e-05
                iter:   52           residual = 9.095541e-06
                iter:   53           residual = 7.434643e-06
                iter:   54           residual = 4.823674e-06
                iter:   55           residual = 2.621504e-06
                iter:   56           residual = 1.837715e-06
                iter:   57           residual = 1.321035e-06
                iter:   58           residual = 1.077122e-06
                iter:   59           residual = 1.064417e-06
                iter:   60           residual = 8.392690e-07
                iter:   61           residual = 5.583901e-07
                iter:   62           residual = 3.338056e-07
                iter:   63           residual = 2.309393e-07
                iter:   64           residual = 1.769351e-07
                iter:   65           residual = 1.401823e-07
                iter:   66           residual = 1.055187e-07
                iter:   67           residual = 7.731635e-08
                iter:   68           residual = 9.196692e-08
                iter:   69           residual = 1.085794e-07
                iter:   70           residual = 1.223716e-07
                iter:   71           residual = 9.632674e-08
                iter:   72           residual = 8.904028e-08
                iter:   73           residual = 6.697396e-08
                iter:   74           residual = 4.398745e-08
                iter:   75           residual = 2.564682e-08
                iter:   76           residual = 1.892250e-08
                iter:   77           residual = 1.327325e-08
                iter:   78           residual = 8.844003e-09


                Solution time: 0.036388 (sec.)
                total iterations: 78
```

<img src="laplace.png"
     alt="Laplace view"
     style="float: left; margin-right: 10px;" />


## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




