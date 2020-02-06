#ifndef SCALAR_FV_SYSTEM_H
#define SCALAR_FV_SYSTEM_H

#include "kernel/base/types.h"
#include "kernel/numerics/scalar_variable.h"
#include "kernel/numerics/dof_manager.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/element.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <memory>

namespace kernel
{

namespace numerics
{

/// forward declarations
template<int dim> class Mesh;
template<int dim> class FVGradBase;

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
class ScalarFVSystem: private boost::noncopyable
{

public:

    typedef AssemblyPolicy assembly_policy_t;
    typedef SolutionPolicy solution_policy_t;
    typedef typename solution_policy_t::matrix_t matrix_t;
    typedef typename solution_policy_t::vector_t vector_t;
    typedef typename solution_policy_t::solver_t solver_t;

    /// \brief constructor
    ScalarFVSystem(std::string&& sys_name, std::string&&  var_name);

    /// \brief constructor
    ScalarFVSystem(std::string&& sys_name, std::string&&  var_name, Mesh<dim>& mesh);

    /// \brief Destructor
    virtual ~ScalarFVSystem();

    /// \brief Distribute the dofs for the system
    /// over the attached mesh
    virtual void distribute_dofs();

    /// \brief Returns the number of dofs
    uint_t n_dofs()const{return dofs_manager_.n_dofs();}

    /// \brief Assemble the system
    virtual void assemble_system();

    /// \brief Solve the system
    virtual void solve();

    /// \brief Save the solution vector
    virtual void save_solution(const std::string& file_name)const;

protected:


    /// \brief The name of the system
    std::string name_;

    /// \brief The variable the system is managing
    ScalarVar var_;

    /// \brief Pointer to the Mesh over which the system will be solved
    Mesh<dim>* m_ptr_;

    /// \brief The object that manages the DoFs for the system
    FVDoFManager<dim> dofs_manager_;

    /// \brief The matrix that assembles the terms
    matrix_t matrix_;

    /// \brief The solution vector
    vector_t solution_;

    /// \brief The rhs vector
    vector_t rhs_;

    /// \brief The assembly ploicy
    assembly_policy_t assembly_;

    /// \brief The solver to solve the system
    solver_t solver_;

};


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy >::ScalarFVSystem(std::string&& sys_name, std::string&&  var_name)
    :
      name_(sys_name),
      var_(var_name),
      m_ptr_(nullptr),
      dofs_manager_(),
      matrix_(),
      solution_(),
      rhs_(),
      assembly_(),
      solver_()
{}


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::ScalarFVSystem(std::string&& sys_name, std::string&&  var_name, Mesh<dim>& mesh)
    :
      name_(sys_name),
      var_(var_name),
      m_ptr_(&mesh),
      dofs_manager_(),
      matrix_(),
      solution_(),
      rhs_(),
      assembly_(),
      solver_()
{}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::~ScalarFVSystem()
{}


template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::distribute_dofs(){


    if(!m_ptr_){
        throw std::logic_error("Mehs pointer is null");
    }

    dofs_manager_.distribute_dofs(*m_ptr_, var_);

}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::assemble_system(){

    matrix_.zero();
    rhs_.zero();
    solution_.zero();

    // loop over the elements
    ConstElementMeshIterator<Active, Mesh<dim>> filter(*m_ptr_);

    auto elem_itr = filter.begin();
    auto elem_itr_e = filter.end();

    for(; elem_itr != elem_itr_e; ++elem_itr){

        auto* elem = *elem_itr;
        assembly_.reinit(*elem);
        assembly_.assemble(matrix_, solution_, rhs_);
    }
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::solve(){

    solver_.solve(matrix_, solution_, rhs_);
}

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
void
ScalarFVSystem<dim, AssemblyPolicy, SolutionPolicy>::save_solution(const std::string& file_name)const{

}

}
}

#endif // SCALAR_FV_SYSTEM_H
