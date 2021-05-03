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
    /// \brief Solve the system
    ///
    virtual solver_output_t solve();

    ///
    /// \brief Save the solution vector
    ///
    virtual void save_solution(const std::string& file_name)const;

    /// \brief Returns the number of dofs
    uint_t n_dofs()const{return dofs_manager_.n_dofs();}

    ///
    /// \brief Set the function that describes the volume term
    ///
    void set_volume_term_function(const NumericScalarFunction<dim>& func){volume_func_ = &func;}

protected:

    ///
    /// \brief Pointer to the Mesh over which the system will be solved
    ///
    Mesh<dim>* m_ptr_;

    /// \brief The object that manages the DoFs for the system
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
      ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>(sys_name, var_name)

      m_ptr_(nullptr),
      dofs_manager_(),
      volume_func_(nullptr)
{}


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::distribute_dofs(){


    if(!m_ptr_){
        throw std::logic_error("Mehs pointer is null");
    }

    dofs_manager_.distribute_dofs(*m_ptr_, var_);

    // initialize the matrix and vector. Policy should take care
    // this
    matrix_.init(dofs_manager_.n_dofs(), dofs_manager_.n_dofs(), 6);
    solution_.init(dofs_manager_.n_dofs(), false);
    rhs_.init(dofs_manager_.n_dofs(), false);

}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::assemble_system(){

    // zero the system entries
    matrix_.zero();
    rhs_.zero();
    solution_.zero();

    assembly_.set_dof_manager(dofs_manager_);

    if(boundary_func_){
        assembly_.set_boundary_function(*boundary_func_);
    }

    if(rhs_func_){
        assembly_.set_rhs_function(*rhs_func_);
    }

    if(volume_func_){
        assembly_.set_volume_term_function(*volume_func_);
    }

    assembly_.set_mesh(*m_ptr_);
    assembly_.assemble(matrix_, solution_, rhs_);
    matrix_.fill_completed();
    solution_.compress();
    rhs_.compress();
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
typename ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::solver_output_t
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::solve(){

    return solver_.solve(matrix_, solution_, rhs_);
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::save_solution(const std::string& file_name)const{

    /// \brief Constructor
    VtkWriter writer(file_name, true);
    writer.write_solution(*m_ptr_, solution_, dofs_manager_);

}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
std::ostream&
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::print_system_matrix(std::ostream& out)const{
    return matrix_.print(out);
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
std::ostream&
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::print_system_rhs(std::ostream& out)const{
    return rhs_.print(out);
}

}
}

#endif
#endif // SCALAR_FV_SYSTEM_H
