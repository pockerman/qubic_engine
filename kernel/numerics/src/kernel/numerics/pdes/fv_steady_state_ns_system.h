#ifndef FV_STEADY_STATE_NS_SYSTEM_H
#define FV_STEADY_STATE_NS_SYSTEM_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#ifdef KERNEL_DEBUG
#include<assert.h>
#endif

#include "kernel/base/types.h"
#include "kernel/numerics/pdes/fv_scalar_system.h"
#include "boost/noncopyable.hpp"

#include <map>
#include <array>
#include <tuple>
namespace kernel {
namespace numerics {

/// forward declarations
template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;

namespace fvm{

///
/// \brief Steady-state Navier-Stokes system in dim-spatial dimensions
/// The template parameters are as follows
///
/// - dim is the spatial dimension of the problem (2 or 3)
/// - VelocitAssemblyPolicy is the policy responsible for assembling the velocity components
/// - PressureAssemblyPolicy is the policy responsible for assembling the pressure component
/// - SolutionPolicy is the policy responsible for solving the ensuing systems
///
template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
class FVSteadyStateNSSystem: private boost::noncopyable
{

public:

    ///
    /// \brief assembly_t
    ///
    typedef AssemblyPolicy  assembly_policy_t;



    ///
    /// \brief vector_t
    ///
    typedef typename SolutionPolicy::vector_t vector_t;

    ///
    /// \brief solver_output_t
    ///
    typedef void solver_output_t;

    ///
    /// \brief Constructor
    ///
    FVSteadyStateNSSystem();

    ///
    /// \brief Destructor
    ///
    ~FVSteadyStateNSSystem()=default;

    ///
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
    virtual solver_output_t solve(const std::string& name);

    ///
    /// \brief Save the solution vector
    ///
    virtual void save_solution(const std::string& file_name)const;

    ///
    /// \brief Returns the total number of dofs
    ///
    uint_t n_dofs()const;

    ///
    /// \brief Set the boundary function
    ///
    void set_boundary_function(const std::string& name, const BoundaryFunctionBase<dim>& func);

    ///
    /// \brief Set the function that describes the rhs
    ///
    void set_rhs_function(const std::string& name, const NumericScalarFunction<dim>& func);

    ///
    /// \brief Set the function that describes the volume term
    ///
    void set_volume_term_function(const std::string& name, const NumericScalarFunction<dim>& func);

    ///
    /// \brief Set the  solver data
    ///
    template<typename SolverDataTp>
    void set_solver_data(const std::string& name, const SolverDataTp& data);

    ///
    /// \brief Returns the assembly policy
    ///
    assembly_policy_t& get_assembly_policy(const std::string& component_name );

    ///
    /// \brief Access the solution vector
    ///
    vector_t& get_solution(const std::string& name);

    ///
    /// \brief Print the matrix
    ///
    std::ostream& print_system_matrix(std::ostream& out)const;

    ///
    /// \brief Print the rhs
    ///
    std::ostream& print_system_rhs(std::ostream& out)const;

    ///
    /// \brief Returns an array of names
    ///
    const std::array<std::string_view, dim> get_names()const;


private:

    ///
    /// \brief v_ The velocity PDES
    ///
    assembly_policy_t assembly_policy_;



};

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
FVSteadyStateNSSystem<dim, AssemblyPolicy, SolutionPolicy>::FVSteadyStateNSSystem()
    :
     assembly_policy_()
{}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
uint_t
FVSteadyStateNSSystem<dim, AssemblyPolicy, SolutionPolicy>::n_dofs()const{

    auto total_dofs = 0; //p_pde_.n_dofs();

    /*auto itr = v_.begin();

    for(; itr != v_.end(); ++itr){
        total_dofs += itr->second.n_dofs();
    }*/

    return total_dofs;
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
FVSteadyStateNSSystem<dim, AssemblyPolicy, SolutionPolicy>::set_boundary_function(const std::string& name,
                                                                                  const BoundaryFunctionBase<dim>& func){

    /*if(name == "P"){
        p_pde_.set_boundary_function(func);
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
    }*/

    throw std::logic_error("Name " + name + "does not exist");
}

/*template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
void
FVSteadyStateNSSystem<dim, VelocitAssemblyPolicy,
                   PressureAssemblyPolicy, SolutionPolicy>::set_rhs_function(const std::string& name,
                                                                             const NumericScalarFunction<dim>& func){

    if(name == "P"){
        p_pde_.set_rhs_function(func);
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
}*/

/*template<int dim, typename VelocitAssemblyPolicy,
         typename PressureAssemblyPolicy, typename SolutionPolicy>
void
FVSteadyStateNSSystem<dim, VelocitAssemblyPolicy,
                   PressureAssemblyPolicy, SolutionPolicy>::set_volume_term_function(const std::string& name,
                                                                                     const NumericScalarFunction<dim>& func){

    if(name == "P"){
        p_pde_.set_volume_term_function(func);
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

#ifdef KERNEL_DEBUG
    std::string msg("Name " + name + "does not exist");
    assert(false && msg.c_str());
#endif

}*/


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
FVSteadyStateNSSystem<dim, AssemblyPolicy, SolutionPolicy>::assemble_system(){

    /// assume that the velocity systems
    /// are independent so we can assemble them
    /// separately
    ///

    assembly_policy_.assemble();

    /*auto v_itr = v_.begin();

    for(; v_itr != v_.end(); ++v_itr){

        v_itr->second.assemble_system();
        v_itr->second.solve_system();
    }

    /// solve for the pressure variable
    p_pde_.assemble_system();
    p_pde_.solve();*/


}

}
}

}
#endif
#endif // STEADY_STATE_NS_SYSTEM_H
