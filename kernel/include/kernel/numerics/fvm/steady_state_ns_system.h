#ifndef STEADY_STATE_NS_SYSTEM_H
#define STEADY_STATE_NS_SYSTEM_H

#include "kernel/base/types.h"
#include "kernel/numerics/fvm/scalar_fv_system.h"
#include "boost/noncopyable.hpp"

#include <map>
#include <array>
#include <tuple>
namespace kernel {
namespace numerics {

template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;


template<int dim, typename VelocitAssemblyPolicy, typename PressureAssemblyPolicy, typename SolutionPolicy>
class SteadyStateNSSystem: private boost::noncopyable
{

public:


    typedef PressureAssemblyPolicy  pressure_assembly_t;

    /// \brief Distribute the dofs for the system
    /// over the attached mesh
    virtual void distribute_dofs();

    /// \brief Assemble the system
    virtual void assemble_system();

    /// \brief Solve the system
    virtual solver_output_t solve(const std::string& name);

    /// \brief Save the solution vector
    virtual void save_solution(const std::string& file_name)const;

    /// \brief Returns the number of dofs
    uint_t n_dofs()const{return dofs_manager_.n_dofs();}

    /// \brief Set the boundary function
    void set_boundary_function(const BoundaryFunctionBase<dim>& func){boundary_func_ = &func;}

    /// \brief Set the function that describes the rhs
    void set_rhs_function(const NumericScalarFunction<dim>& func){rhs_func_ = &func;}

    /// \brief Set the function that describes the volume term
    void set_volume_term_function(const NumericScalarFunction<dim>& func){volume_func_ = &func;}

    /// \brief Set the  solver data
    template<typename SolverDataTp>
    void set_solver_data(const SolverDataTp& data){solver_.set_solver_data(data);}

    /// \brief Returns the assembly policy
    assembly_policy_t& get_velocity_assembly_policy(const std::string& component_name );

    /// \brief Returns the assembly policy for the pressure
    pressure_assembly_t& get_pressure_assembly_policy();

    /// \brief Access the solution vector
    vector_t& get_solution(){return solution_;}

    /// \brief Print the matrix
    std::ostream& print_system_matrix(std::ostream& out)const;

    /// \brief Print the rhs
    std::ostream& print_system_rhs(std::ostream& out)const;


    /// \brief Raturns an array of names
    const std::array<std::string_view, dim> get_names()const;


private:


    std::map<std::string, ScalarFVSystem<dim, VelocitAssemblyPolicy, SolutionPolicy>> v_;
    ScalarFVSystem<dim, PressureAssemblyPolicy, SolutionPolicy> p_;

};

}

}

#endif // STEADY_STATE_NS_SYSTEM_H
