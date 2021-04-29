#ifndef FV_STEADY_STATE_NS_SYSTEM_H
#define FV_STEADY_STATE_NS_SYSTEM_H

#include "kernel/base/types.h"
#include "kernel/numerics/fvm/fv_scalar_system.h"
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
    typedef VelocitAssemblyPolicy   velocity_assembly_policy_t;
    typedef typename SolutionPolicy::vector_t vector_t;
    typedef void solver_output_t;

    /// \brief Constructor
    SteadyStateNSSystem();

    /// \brief Destructor
    ~SteadyStateNSSystem();

    /// \brief Distribute the dofs for the system
    /// over the attached mesh
    virtual void distribute_dofs();

    /// \brief Assemble the system
    virtual void assemble_system();

    /// \brief Solve the system
    virtual solver_output_t solve(const std::string& name);

    /// \brief Save the solution vector
    virtual void save_solution(const std::string& file_name)const;

    /// \brief Returns the total number of dofs
    uint_t n_dofs()const;

    /// \brief Set the boundary function
    void set_boundary_function(const std::string& name, const BoundaryFunctionBase<dim>& func);

    /// \brief Set the function that describes the rhs
    void set_rhs_function(const std::string& name, const NumericScalarFunction<dim>& func);

    /// \brief Set the function that describes the volume term
    void set_volume_term_function(const std::string& name, const NumericScalarFunction<dim>& func);

    /// \brief Set the  solver data
    template<typename SolverDataTp>
    void set_solver_data(const std::string& name, const SolverDataTp& data);

    /// \brief Returns the assembly policy
    velocity_assembly_policy_t& get_velocity_assembly_policy(const std::string& component_name );

    /// \brief Returns the assembly policy for the pressure
    pressure_assembly_t& get_pressure_assembly_policy();

    /// \brief Access the solution vector
    vector_t& get_solution(const std::string& name);

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

template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
SteadyStateNSSystem<dim, VelocitAssemblyPolicy, PressureAssemblyPolicy, SolutionPolicy>::SteadyStateNSSystem()
    :
      v_(),
      p_()
{}

template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
uint_t
SteadyStateNSSystem<dim, VelocitAssemblyPolicy, PressureAssemblyPolicy, SolutionPolicy>::n_dofs()const{

    auto total_dofs = p_.n_dofs();

    auto itr = v_.begin();

    for(; itr != v_.end(); ++itr){
        total_dofs += itr->second.n_dofs();
    }

    return total_dofs;
}

template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
void
SteadyStateNSSystem<dim, VelocitAssemblyPolicy,
                   PressureAssemblyPolicy, SolutionPolicy>::set_boundary_function(const std::string& name,
                                                                                  const BoundaryFunctionBase<dim>& func){

    if(name == "P"){
        p_.set_boundary_function(func);
        return;
    }
    else{

        auto itr = v_.begin();
        for(; itr != v_.end(); ++itr){

            if(itr->first == name){
                itr->second.set_boundary_function(func);
                return;
            }
        }
    }

    throw std::logic_error("Name " + name + "does not exist");
}

template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
void
SteadyStateNSSystem<dim, VelocitAssemblyPolicy,
                   PressureAssemblyPolicy, SolutionPolicy>::set_rhs_function(const std::string& name,
                                                                             const NumericScalarFunction<dim>& func){

    if(name == "P"){
        p_.set_rhs_function(func);
        return;
    }
    else{

        auto itr = v_.begin();
        for(; itr != v_.end(); ++itr){

            if(itr->first == name){
                itr->second.set_rhs_function(func);
                return;
            }
        }
    }

    throw std::logic_error("Name " + name + "does not exist");
}

template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
void
SteadyStateNSSystem<dim, VelocitAssemblyPolicy,
                   PressureAssemblyPolicy, SolutionPolicy>::set_volume_term_function(const std::string& name,
                                                                                     const NumericScalarFunction<dim>& func){

    if(name == "P"){
        p_.set_volume_term_function(func);
        return;
    }
    else{

        auto itr = v_.begin();

        for(; itr != v_.end(); ++itr){

            if(itr->first == name){
                itr->second.set_volume_term_function(func);
                return;
            }

        }
    }

    throw std::logic_error("Name " + name + "does not exist");

}


template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
void
SteadyStateNSSystem<dim, VelocitAssemblyPolicy,
PressureAssemblyPolicy, SolutionPolicy>::assemble_system(){

    /// assume that the velocity systems
    /// are independent so we can assemble them
    /// separately

    auto v_itr = v_.begin();

    for(; v_itr != v_.end(); ++v_itr){

        v_itr->second.assemble_system();
        v_itr->second.solve_system();
    }

    /// solve for the pressure variable
    p_.assemble_system();
    p_.solve();


}

}

}

#endif // STEADY_STATE_NS_SYSTEM_H
