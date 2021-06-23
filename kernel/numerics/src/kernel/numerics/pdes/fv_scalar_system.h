#ifndef SCALAR_FV_SYSTEM_H
#define SCALAR_FV_SYSTEM_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/pdes/scalar_system_base.h"
#include "kernel/base/types.h"
#include "kernel/numerics/scalar_variable.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_data.h"
#include "kernel/discretization/dof_manager.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/vtk_writer.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <memory>
#include <iostream>

namespace kernel{
namespace numerics{


/// forward declarations
template<int dim> class Mesh;
template<int dim> class FVGradBase;
template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;

namespace pdes{

///
/// \brief
///
template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
class ScalarFVSystem: public pdes::ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>
{

public:

    typedef AssemblyPolicy assembly_policy_t;
    typedef SolutionPolicy solution_policy_t;
    typedef typename solution_policy_t::matrix_t matrix_t;
    typedef typename solution_policy_t::vector_t vector_t;
    typedef typename solution_policy_t::solver_t solver_t;
    typedef typename solution_policy_t::solver_output_t solver_output_t;

    ///
    /// \brief constructor
    ///
    ScalarFVSystem(const std::string& sys_name, const std::string&  var_name);

    ///
    /// \brief Destructor
    ///
    virtual ~ScalarFVSystem();

    /// \brief Distribute the dofs for the system
    /// over the attached mesh
    ///
    virtual void distribute_dofs();

    ///
    /// \brief Assemble the system
    ///
    virtual void assemble_system();

    ///
    /// \brief Save the solution vector
    ///
    virtual void save_solution(const std::string& file_name)const;

    ///
    /// \brief Returns the number of dofs
    ///
    uint_t n_dofs()const{return dofs_manager_.n_dofs();}

    ///
    /// \brief Set the function that describes the volume term
    ///
    void set_volume_term_function(const NumericScalarFunction<dim>& func){volume_func_ = &func;}

protected:

    ///
    /// \brief The object that manages the DoFs for the system
    ///
    FVDoFManager<dim> dofs_manager_;

    ///
    /// \brief Pointer to the function object that describes the
    /// any volume terms to assemble
    ///
    const NumericScalarFunction<dim>* volume_func_;

};


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy >::ScalarFVSystem(const std::string& sys_name, const std::string&  var_name)
    :
      ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>(sys_name, var_name),
      dofs_manager_(),
      volume_func_(nullptr)
{}


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::distribute_dofs(){

    dofs_manager_.distribute_dofs(this->get_mesh(), this->get_variable());

    // initialize the matrix and vector. Policy should take care
    // this
    this->get_matrix().init(dofs_manager_.n_dofs(), dofs_manager_.n_dofs(), 6);
    this->get_solution().init(dofs_manager_.n_dofs(), false);
    this->get_rhs_vector().init(dofs_manager_.n_dofs(), false);

}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::assemble_system(){

    // zero the system entries
   this->zero_linear_algebra_objects();

   this->get_assembly_policy().set_dof_manager(dofs_manager_);

    /*if(boundary_func_){
        assembly_.set_boundary_function(*boundary_func_);
    }

    if(rhs_func_){
        assembly_.set_rhs_function(*rhs_func_);
    }*/

   if(volume_func_){
       this->get_assembly_policy().set_volume_term_function(*volume_func_);
   }

   // call base class to assemble
   this->ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>::assemble_system();

   /*assembly_.set_mesh(*m_ptr_);
   assembly_.assemble(matrix_, solution_, rhs_);
   matrix_.fill_completed();
   solution_.compress();
   rhs_.compress();*/
}


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::save_solution(const std::string& file_name)const{

    /// \brief Constructor
    VtkWriter writer(file_name, true);
    writer.write_solution(this->get_mesh(),  this->get_solution(), dofs_manager_);

}



}
}
}

#endif
#endif // SCALAR_FV_SYSTEM_H
