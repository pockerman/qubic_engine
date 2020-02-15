#ifndef SIMPLE_FV_TIME_STEPPER_H
#define SIMPLE_FV_TIME_STEPPER_H

#include "kernel/base/types.h"
#include "kernel/base/config.h"
#include "kernel/numerics/dof.h"

namespace kernel {
namespace numerics{

/// forward declarations
template<int dim> class Element;
template<int dim> class Mesh;
template<int dim> class FVDoFManager;

#ifdef USE_TRILINOS
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;
#endif

template<int dim>
class SimpleFVTimeAssemblyPolicy
{
public:

    /// \brief Constructor
    SimpleFVTimeAssemblyPolicy();

    /// \brief Reinitialize the policy
    void reinit(const Element<dim>& element);


#ifdef USE_TRILINOS

    /// \brief Assemble the data
    void assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b,
                  const std::vector<TrilinosEpetraVector>& old_solutions);

    /// \brief assemble one element contribution
    void assemble_one_element(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b,
                              const std::vector<TrilinosEpetraVector>& old_solutions);

#endif

    /// \brief Set the object that describes the dofs
    void set_dof_manager(const FVDoFManager<dim>& dof_manager){dof_manager_ = &dof_manager;}

    /// \brief Set the mesh pointer
    void set_mesh(const Mesh<dim>& mesh){m_ptr_ = &mesh;}

    /// \brief Set the time step size
    void set_time_step(real_t dt){dt_ = dt;}

    /// \brief Returns the time step
    real_t time_step()const{return dt_;}

    /// \brief set how many old solution vectors the stepper is using
    void set_n_old_solution_vectors(uint_t nsols){nsols_ = nsols;}

    uint_t n_old_solution_vectors()const{return nsols_;}

private:

    /// \brief The time step
    real_t dt_;

    uint_t nsols_;

    /// \brief The element over which the policy is working
    const Element<dim>* elem_;

    /// \brief The dofs of the neighbors of the cell we do work on
    std::vector<DoF> neigh_dofs_;

    /// \brief The dofs of the cell
    std::vector<DoF> cell_dofs_;

    /// \brief The DoFManager that handles the dofs
    const FVDoFManager<dim>* dof_manager_;

    /// \brief The Mesh over which the policy is working
    const Mesh<dim>* m_ptr_;

    /// \brief initialize dofs
    void initialize_dofs_();
};


}

}

#endif // SIMPLE_FV_TIME_STEPPER_H
