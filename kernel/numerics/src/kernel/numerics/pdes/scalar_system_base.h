#ifndef SCALAR_SYSTEM_BASE_H
#define SCALAR_SYSTEM_BASE_H


#include "kernel/base/types.h"
#include "kernel/numerics/scalar_variable.h"
#include <boost/noncopyable.hpp>
#include <string>
#include <memory>
#include <iostream>

namespace kernel {
namespace numerics {

/// forward declarations
template<int dim> class Mesh;
template<int dim> class FVGradBase;
template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;

namespace pdes {



///
/// \brief ScalarSystemBase
///
template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
class ScalarSystemBase: private boost::noncopyable
{
public:

    typedef AssemblyPolicy assembly_policy_t;
    typedef SolutionPolicy solution_policy_t;
    typedef typename solution_policy_t::matrix_t matrix_t;
    typedef typename solution_policy_t::vector_t vector_t;
    typedef typename solution_policy_t::solver_t solver_t;
    typedef typename solution_policy_t::solver_output_t solver_output_t;

    ///
    /// \brief Destructor
    ///
    virtual ~ScalarSystemBase()=default;

    ///
    /// \brief Distribute the dofs for the system
    /// over the attached mesh
    ///
    virtual void distribute_dofs()=0;

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
    virtual void save_solution(const std::string& file_name)const=0;

    ///
    /// \brief Returns the number of dofs
    ///
    virtual uint_t n_dofs()const=0;

    ///
    /// \brief Set the boundary function
    ///
    void set_boundary_function(const BoundaryFunctionBase<dim>& func){boundary_func_ = &func;}

    ///
    /// \brief Set the function that describes the rhs
    ///
    void set_rhs_function(const NumericScalarFunction<dim>& func){rhs_func_ = &func;}

    ///
    /// \brief Set the  solver data
    ///
    template<typename SolverDataTp>
    void set_solver_data(const SolverDataTp& data){solver_.set_solver_data(data);}

    ///
    /// \brief Returns the assembly policy
    ///
    assembly_policy_t& get_assembly_policy(){return assembly_;}

    ///
    /// \brief Access the solution vector
    ///
    vector_t& get_solution(){return solution_;}

    ///
    /// \brief Print the matrix
    ///
    std::ostream& print_system_matrix(std::ostream& out)const;

    ///
    /// \brief Print the rhs
    ///
    std::ostream& print_system_rhs(std::ostream& out)const;

    ///
    /// \brief get_variable
    /// \return
    ///
    ScalarVar& get_variable(){return var_;}

    ///
    /// \brief get_mesh
    /// \return
    ///
    Mesh<dim>& get_mesh(){return *m_ptr_;}

    ///
    /// \brief get_matrix
    /// \return
    ///
    matrix_t& get_matrix(){return matrix_; }

    ///
    /// \brief get_rhs_vector
    /// \return
    ///
    vector_t& get_rhs_vector(){return rhs_;}

    ///
    /// \brief zero_linear_algebra_objects
    ///
    void zero_linear_algebra_objects();


protected:

    ///
    /// \brief constructor
    ///
    ScalarSystemBase(const std::string& sys_name, const std::string&  var_name);

    ///
    /// \brief The name of the system
    ///
    std::string name_;

    ///
    /// \brief The variable the system is managing
    ///
    ScalarVar var_;

    ///
    /// \brief Pointer to the Mesh over which the system will be solved
    ///
    Mesh<dim>* m_ptr_;

    ///
    /// \brief The matrix that assembles the terms
    ///
    matrix_t matrix_;

    ///
    /// \brief The solution vector
    ///
    vector_t solution_;

    /// \brief The rhs vector
    vector_t rhs_;

    ///
    /// \brief The assembly ploicy
    ///
    assembly_policy_t assembly_;

    ///
    /// \brief The solver to solve the system
    ///
    solver_t solver_;

    ///
    /// \brief The object that describes the boundary conditions
    ///
    const BoundaryFunctionBase<dim>* boundary_func_;

    ///
    /// \brief The object that describes the rhs
    ///
    const NumericScalarFunction<dim>* rhs_func_;

};

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>::zero_linear_algebra_objects(){

    // zero the system entries
    matrix_.zero();
    rhs_.zero();
    solution_.zero();
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>::assemble_system(){

    if(boundary_func_){
        assembly_.set_boundary_function(*boundary_func_);
    }

    if(rhs_func_){
        assembly_.set_rhs_function(*rhs_func_);
    }

    /*if(volume_func_){
        assembly_.set_volume_term_function(*volume_func_);
    }*/

    assembly_.set_mesh(*m_ptr_);
    assembly_.assemble(matrix_, solution_, rhs_);
    matrix_.fill_completed();
    solution_.compress();
    rhs_.compress();
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
typename ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>::solver_output_t
ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>::solve(){
    return solver_.solve(matrix_, solution_, rhs_);
}


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
std::ostream&
ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>::print_system_matrix(std::ostream& out)const{
    return matrix_.print(out);
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
std::ostream&
ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>::print_system_rhs(std::ostream& out)const{
    return rhs_.print(out);
}

}

}

}

#endif // SCALAR_SYSTEM_BASE_H
