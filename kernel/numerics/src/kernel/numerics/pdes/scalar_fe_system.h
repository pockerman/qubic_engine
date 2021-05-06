#ifndef SCALAR_FE_SYSTEM_H
#define SCALAR_FE_SYSTEM_H

#include "kernel/base/config.h"

#ifdef USE_FEM

#include "kernel/numerics/pdes/scalar_system_base.h"


namespace kernel {
namespace numerics {
namespace fem {

template<int dim, typename AssemblyPolicy, typename SolutionPolicy>
class ScalarFESystem: public pdes::ScalarSystemBase<dim, AssemblyPolicy, SolutionPolicy>
{
public:

    typedef AssemblyPolicy assembly_policy_t;
    typedef SolutionPolicy solution_policy_t;
    typedef typename solution_policy_t::matrix_t matrix_t;
    typedef typename solution_policy_t::vector_t vector_t;
    typedef typename solution_policy_t::solver_t solver_t;
    typedef typename solution_policy_t::solver_output_t solver_output_t;

    ///
    /// \brief Distribute the dofs for the system
    /// over the attached mesh
    ///
    virtual void distribute_dofs()=0;

    ///
    /// \brief Assemble the system
    ///
    virtual void assemble_system()=0;

    ///
    /// \brief Solve the system
    ///
    virtual solver_output_t solve()=0;

    ///
    /// \brief Save the solution vector
    ///
    virtual void save_solution(const std::string& file_name)const;

    ///
    /// \brief Returns the number of dofs
    ///
    virtual uint_t n_dofs()const=0;


};

}

}

}

#endif
#endif // SCALAR_FE_SYSTEM_H
