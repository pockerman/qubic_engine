#ifndef FV_SCALAR_TIMED_SYSTEM_H
#define FV_SCALAR_TIMED_SYSTEM_H

#include "kernel/base/config.h"

#ifdef USE_FVM

#include "kernel/numerics/pdes/fv_scalar_system.h"
#include <vector>

namespace kernel {
namespace numerics {

template<int dim, typename TimeStepper, typename AssemblePolicy, typename SolutionPolicy>
class FVScalarTimedSystem: public ScalarFVSystem<dim, AssemblePolicy, SolutionPolicy>
{
public:

    typedef ScalarFVSystem<dim, AssemblePolicy, SolutionPolicy> base_t;
    typedef typename base_t::assembly_policy_t assembly_policy_t;
    typedef typename base_t::solution_policy_t solution_policy_t;
    typedef typename solution_policy_t::matrix_t matrix_t;
    typedef typename solution_policy_t::vector_t vector_t;
    typedef typename solution_policy_t::solver_t solver_t;
    typedef typename solution_policy_t::solver_output_t solver_output_t;
    typedef TimeStepper time_stepper_t;

    /// \brief constructor
    FVScalarTimedSystem(std::string&& sys_name, std::string&&  var_name);

    /// \brief constructor
    FVScalarTimedSystem(std::string&& sys_name, std::string&&  var_name, Mesh<dim>& mesh);

    /// \brief Destructor
    virtual ~FVScalarTimedSystem();

    /// \brief Distribute the dofs for the system
    /// over the attached mesh
    virtual void distribute_dofs();

    /// \brief Assemble the system
    virtual void assemble_system();

    /// \brief Solve the system
    virtual solver_output_t solve();

    /// \brief Save the solution vector
    virtual void save_solution(const std::string& file_name)const;

    /// \brief Set the time stepper data
    void set_time_step(real_t dt){stepper_.set_time_step(dt);}

    /// \brief Set the number of the old solution vectors
    void set_n_old_solution_vectors(uint_t nsols){stepper_.set_n_old_solution_vectors(nsols);}

    /// \brief Return the i-th old solution vector
    vector_t& get_old_solution_vector(uint_t s){return old_solutions_[s];}

protected:

    /// \brief The old solutions vector
    std::vector<vector_t> old_solutions_;

    /// \brief the object responsible for time stepping
    time_stepper_t stepper_;

};

template<int dim, typename TimeStepper, typename AssemblyPolicy, typename SolutionPolicy>
FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy>::FVScalarTimedSystem(std::string&& sys_name, std::string&&  var_name)
    :
   ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>(sys_name, var_name),
   old_solutions_(),
   stepper_()
{}


template<int dim, typename TimeStepper, typename AssemblyPolicy, typename SolutionPolicy>
FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy >::FVScalarTimedSystem(std::string&& sys_name, std::string&&  var_name, Mesh<dim>& mesh)
    :
      ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>(std::move(sys_name), std::move(var_name), mesh),
      old_solutions_(),
      stepper_()
{}

template<int dim, typename TimeStepper, typename AssemblyPolicy, typename SolutionPolicy>
FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy >::~FVScalarTimedSystem()
{}


template<int dim, typename TimeStepper, typename AssemblyPolicy, typename SolutionPolicy>
void
FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy >::distribute_dofs(){


    if(!this->m_ptr_){
        throw std::logic_error("Mehs pointer is null");
    }

    this->dofs_manager_.distribute_dofs(*this->m_ptr_, this->var_);

    // let's initialize the matrix and vector
    this->matrix_.init(this->dofs_manager_.n_dofs(), this->dofs_manager_.n_dofs(), 6);
    this->solution_.init(this->dofs_manager_.n_dofs(), false);
    this->rhs_.init(this->dofs_manager_.n_dofs(), false);

    old_solutions_.resize(stepper_.n_old_solution_vectors());

    for(uint_t v= 0; v<old_solutions_.size(); ++v){
        old_solutions_[v].init(this->dofs_manager_.n_dofs(), false);
    }

}

template<int dim, typename TimeStepper, typename AssemblyPolicy, typename SolutionPolicy>
void
FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy >::assemble_system(){

    // zero the system entries
    this->matrix_.zero();
    this->rhs_.zero();
    this->solution_.zero();

    this->assembly_.set_dof_manager(this->dofs_manager_);
    stepper_.set_dof_manager(this->dofs_manager_);

    if(this->boundary_func_){
        this->assembly_.set_boundary_function(*this->boundary_func_);
    }

    if(this->rhs_func_){
        this->assembly_.set_rhs_function(*this->rhs_func_);
    }

    if(this->volume_func_){
        this->assembly_.set_volume_term_function(*this->volume_func_);
    }

    this->assembly_.set_mesh(*this->m_ptr_);
    this->assembly_.assemble(this->matrix_, this->solution_, this->rhs_);

    stepper_.set_mesh(*this->m_ptr_);
    stepper_.assemble(this->matrix_, this->solution_, this->rhs_, old_solutions_);

    this->matrix_.fill_completed();
    this->solution_.compress();
    this->rhs_.compress();
}

template<int dim, typename TimeStepper, typename AssemblyPolicy, typename SolutionPolicy>
typename FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy >::solver_output_t
FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy >::solve(){

    return this->solver_.solve(this->matrix_, this->solution_, this->rhs_);
}

template<int dim, typename TimeStepper, typename AssemblyPolicy, typename SolutionPolicy>
void
FVScalarTimedSystem<dim, TimeStepper, AssemblyPolicy, SolutionPolicy >::save_solution(const std::string& file_name)const{

    /// \brief Constructor
    VtkWriter writer(file_name, true);
    writer.write_solution(*this->m_ptr_, this->solution_, this->dofs_manager_);

}


}
}

#endif
#endif // FV_SCALAR_TIMED_SYSTEM_H
