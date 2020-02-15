# Example 22: Use Volume Terms With FVM

## Contents

* [Overview](#overview)
* [Upwind scheme](#upwind_scheme)
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="overview"></a> Overview

## <a name="upwind_scheme"></a> Upwind scheme

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

#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#include "kernel/maths/krylov_solvers/trilinos_krylov_solver.h"
#include "kernel/maths/krylov_solvers/krylov_solver_data.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"
#include "kernel/maths/functions/numeric_scalar_function.h"
#include "kernel/maths/functions/numeric_vector_function.h"

#include "kernel/numerics/scalar_dirichlet_bc_function.h"
#include "kernel/numerics/fv_grad_factory.h"
#include "kernel/numerics/fv_grad_types.h"
#include "kernel/numerics/scalar_fv_system.h"
#include "kernel/numerics/trilinos_solution_policy.h"
#include "kernel/numerics/fv_laplace_assemble_policy.h"
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
using kernel::DynVec;
using kernel::numerics::Mesh;
using kernel::GeomPoint;
using kernel::numerics::ScalarFVSystem;
using kernel::numerics::TrilinosSolutionPolicy;
using kernel::numerics::FVLaplaceAssemblyPolicy;
using kernel::numerics::ScalarDirichletBCFunc;
using kernel::numerics::KrylovSolverData;

const real_t width = 1.0/8.0;

GeomPoint<2> source_center(uint_t i){

  std::vector<real_t> values(2, 0.0);
  switch(i)
  {
    case 0:
       values[0] = -0.5;
       values[1] = 0.5;
      break;
    case 1:
       values[0] = -0.5;
       values[1] = -0.5;
      break;
    case 2:
       values[0] = 0.5;
       values[1] = -0.5;
      break;

  }

 return GeomPoint<2>(values);

}

class RhsVals: public kernel::numerics::NumericScalarFunction<2>
{
public:

    /// \brief Returns the value of the function
    virtual real_t value(const GeomPoint<2>&  input)const override final;
};

real_t
RhsVals::value(const GeomPoint<2>& point)const{

    real_t value=0.0;

    for(uint_t i=0; i<3; i++){

      const GeomPoint<2> x_minus_xi = point - source_center(i);

      value += ((2*2 - 4*x_minus_xi.square_sum()/(width *width)) /(width *width) *std::exp(-x_minus_xi.square_sum() /(width *width)));
      value += std::exp(-x_minus_xi.square_sum()/(width*width));
    }

    return value;


}

class VolumeVals: public kernel::numerics::NumericScalarFunction<2>
{
public:

    /// \brief Returns the value of the function
    virtual real_t value(const GeomPoint<2>&  input)const override final;
};

real_t
VolumeVals::value(const GeomPoint<2>& /*point*/)const{
    return 1.0;
}


// The BC description
class BoundaryValues: public kernel::numerics::BoundaryFunctionBase<2>
{
public:

    typedef kernel::numerics::BoundaryFunctionBase<2>::input_t input_t;
    typedef kernel::numerics::BoundaryFunctionBase<2>::output_t output_t;

    // constructor
    BoundaryValues();

    //
    // \brief Returns the value of the function on the Dirichlet boundary
    virtual output_t value(const GeomPoint<2>&  p)const override;
    virtual DynVec<real_t> gradients(const GeomPoint<2> & p)const override;

};

BoundaryValues::BoundaryValues()
    :
    kernel::numerics::BoundaryFunctionBase<2>()
{
    this->set_bc_type(0, kernel::numerics::BCType::NEUMANN);
    this->set_bc_type(1, kernel::numerics::BCType::DIRICHLET);
    this->set_bc_type(2, kernel::numerics::BCType::DIRICHLET);
    this->set_bc_type(3, kernel::numerics::BCType::NEUMANN);
}

BoundaryValues::output_t
BoundaryValues::value(const GeomPoint<2>&  p)const{

    real_t return_value = 0;

    for (uint_t i=0; i<3; i++)
    {
       const GeomPoint<2> x_minus_xi = p - source_center(i);
       return_value += std::exp(-x_minus_xi.square_sum() /(width*width));
    }

    return return_value;
}

DynVec<real_t>
BoundaryValues::gradients(const GeomPoint<2> & p)const{

    DynVec<real_t>  return_value(2, 0.0);

    for (unsigned int i=0; i<3; i++)
     {
          const GeomPoint<2> x_minus_xi = p - source_center(i);

           return_value[0] += (-2 /(width*width) *
                            std::exp(-x_minus_xi.square_sum() /
                           (width*width)) *x_minus_xi[0]);

           return_value[1] += (-2 /(width*width) *
                            std::exp(-x_minus_xi.square_sum() /
                           (width*width)) *x_minus_xi[1]);


    }

    return return_value;
}

}

int main(){

    using namespace example;

    try{

        Mesh<2> mesh;

        {
            uint_t nx = 100;
            uint_t ny = 100;

            GeomPoint<2> start(-1.0);
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
        BoundaryValues bc_func;

        // description of the RHS
        RhsVals rhs;

        // description of the volume term
        VolumeVals vol_vals;

        // laplace system
        ScalarFVSystem<2, FVLaplaceAssemblyPolicy<2>, TrilinosSolutionPolicy> helm("Helm", "U", mesh);

        // system configuration
        helm.set_boundary_function(bc_func);
        helm.set_rhs_function(rhs);
        helm.set_solver_data(data);
        helm.set_volume_term_function(vol_vals);

        auto grad_builder = [](){
            return kernel::numerics::FVGradFactory<2>::build(kernel::numerics::FVGradType::GAUSS);
        };

        helm.get_assembly_policy().build_gradient(grad_builder);

        // distribute the dofs
        helm.distribute_dofs();
        std::cout<<"Number of dofs: "<<helm.n_dofs()<<std::endl;

        helm.assemble_system();
        helm.solve();
        helm.save_solution("helm_system.vtk");

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
Number of dofs: 10000

                *******************************************************
                ***** Problem: Epetra::CrsMatrix
                ***** Preconditioned GMRES solution
                ***** ILU(0) domain decomp. without overlap
                ***** No scaling
                ***** NOTE: convergence VARIES when the total number of
                *****       processors is changed.
                *******************************************************

                iter:    0           residual = 1.000000e+00

*********************************************************************
*****  Condition number estimate for subdomain preconditioner on PE 0 = 2.6422e+00
*********************************************************************
                iter:    1           residual = 4.308928e-01
                iter:    2           residual = 2.432915e-01
                iter:    3           residual = 1.565491e-01
                iter:    4           residual = 1.093301e-01
                iter:    5           residual = 8.228524e-02
                iter:    6           residual = 6.334569e-02
                iter:    7           residual = 5.080178e-02
                iter:    8           residual = 3.897126e-02
                iter:    9           residual = 3.064067e-02
                iter:   10           residual = 2.475900e-02
                iter:   11           residual = 2.028881e-02
                iter:   12           residual = 1.650773e-02
                iter:   13           residual = 1.349504e-02
                iter:   14           residual = 1.114470e-02
                iter:   15           residual = 9.273215e-03
                iter:   16           residual = 7.862394e-03
                iter:   17           residual = 6.768251e-03
                iter:   18           residual = 5.954197e-03
                iter:   19           residual = 5.302563e-03
                iter:   20           residual = 4.773384e-03
                iter:   21           residual = 4.393021e-03
                iter:   22           residual = 4.058150e-03
                iter:   23           residual = 3.823872e-03
                iter:   24           residual = 3.619573e-03
                iter:   25           residual = 3.432935e-03
                iter:   26           residual = 3.260906e-03
                iter:   27           residual = 3.095444e-03
                iter:   28           residual = 2.897686e-03
                iter:   29           residual = 2.709338e-03
                iter:   30           residual = 2.563793e-03
                iter:   31           residual = 2.502839e-03
                iter:   32           residual = 2.433820e-03
                iter:   33           residual = 2.370586e-03
                iter:   34           residual = 2.312857e-03
                iter:   35           residual = 2.259541e-03
                iter:   36           residual = 2.200854e-03
                iter:   37           residual = 2.134791e-03
                iter:   38           residual = 2.045236e-03
                iter:   39           residual = 1.939797e-03
                iter:   40           residual = 1.829937e-03
                iter:   41           residual = 1.724259e-03
                iter:   42           residual = 1.626939e-03
                iter:   43           residual = 1.531708e-03
                iter:   44           residual = 1.436591e-03
                iter:   45           residual = 1.334549e-03
                iter:   46           residual = 1.244729e-03
                iter:   47           residual = 1.148736e-03
                iter:   48           residual = 1.062544e-03
                iter:   49           residual = 9.761766e-04
                iter:   50           residual = 8.911580e-04

...
```

<img src="2d_helm.png"
     alt="Solution view 2D"
     style="float: left; margin-right: 10px;" />

<img src="3d_helm.png"
     alt="Solution view 3D"
     style="float: left; margin-right: 10px;" />


## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>




