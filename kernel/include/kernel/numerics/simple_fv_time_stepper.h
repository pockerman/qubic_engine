#ifndef SIMPLE_FV_TIME_STEPPER_H
#define SIMPLE_FV_TIME_STEPPER_H

#include "kernel/base/types.h"
#include "kernel/base/config.h"
#include "kernel/numerics/dof.h"
#include "kernel/numerics/dof_manager.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/mesh.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/discretization/element_mesh_iterator.h"

#ifdef USE_TRILINOS
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#endif

namespace kernel {
namespace numerics{

/// forward declarations
template<int dim> class Element;
template<int dim> class Mesh;
template<int dim> class FVDoFManager;
template<int dim> class BoundaryFunctionBase;
template<int dim> class NumericScalarFunction;

#ifdef USE_TRILINOS
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;
#endif

template<int dim, typename SpatialAssembly>
class BackwardEulerFVTimeAssemblyPolicy
{
public:

    typedef SpatialAssembly spatial_assembly_t;

    /// \brief Constructor
    BackwardEulerFVTimeAssemblyPolicy();

    /// \brief Reinitialize the policy
    void reinit(const Element<dim>& element);

    /// \brief initialize dofs
    void initialize_dofs();

#ifdef USE_TRILINOS

    /// \brief Assemble the data
    void assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b,
                  const std::vector<TrilinosEpetraVector>& old_solutions);

    /// \brief assemble one element contribution
    void assemble_one_element(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x, TrilinosEpetraVector& b,
                              const std::vector<TrilinosEpetraVector>& old_solutions);

#endif

    /// \brief Set the object that describes the dofs
    void set_dof_manager(const FVDoFManager<dim>& dof_manager)
    {spatial_assembly_.set_dof_manager(dof_manager);}

    /// \brief Set the mesh pointer
    void set_mesh(const Mesh<dim>& mesh)
    {spatial_assembly_.set_mesh(mesh);}

    /// \brief Set the time step size
    void set_time_step(real_t dt){dt_ = dt;}

    /// \brief Returns the time step
    real_t time_step()const{return dt_;}

    /// \brief set how many old solution vectors the stepper is using
    void set_n_old_solution_vectors(uint_t nsols){nsols_ = nsols;}

    /// \brief How many solution vectors are cached as old
    /// solutions for this policy
    uint_t n_old_solution_vectors()const{return nsols_;}

    /// \brief Set the function that describes the boundary conditions
    void set_boundary_function(const BoundaryFunctionBase<dim>& func)
    {spatial_assembly_.set_boundary_function(func);}

    /// \brief Set the function that describes the boundary conditions
    void set_rhs_function(const NumericScalarFunction<dim>& func)
    {spatial_assembly_.set_rhs_function(func);}

    /// \brief Set the function that describes the boundary conditions
    void set_volume_term_function(const NumericScalarFunction<dim>& func)
    {spatial_assembly_.set_volume_term_function(func);}

    /// \brief Read/write access to the assembly policy
    spatial_assembly_t& get_assembly_polcy(){return spatial_assembly_;}

private:

    /// \brief The object responsible for the
    /// spatial assembly
    spatial_assembly_t spatial_assembly_;

    /// \brief The time step
    real_t dt_;

    /// \brief how many solutions vectors are cached
    uint_t nsols_;

};


template<int dim,typename SpatialAssembly>
BackwardEulerFVTimeAssemblyPolicy<dim,SpatialAssembly>::BackwardEulerFVTimeAssemblyPolicy()
    :
      spatial_assembly_(),
      dt_(0.1),
      nsols_(0)
{}

template<int dim,typename SpatialAssembly>
void
BackwardEulerFVTimeAssemblyPolicy<dim, SpatialAssembly>::initialize_dofs(){
    spatial_assembly_.initialze_dofs();
}

template<int dim,typename SpatialAssembly>
void
BackwardEulerFVTimeAssemblyPolicy<dim, SpatialAssembly>::reinit(const Element<dim>& element){
   spatial_assembly_.reinit(element);
}

#ifdef USE_TRILINOS

template<int dim,typename SpatialAssembly>
void
SimpleFVTimeAssemblyPolicy<dim, SpatialAssembly>::assemble(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x,
                                          TrilinosEpetraVector& b, const std::vector<TrilinosEpetraVector>& old_solutions ){

    // loop over the elements
    ConstElementMeshIterator<Active, Mesh<dim>> filter(spatial_assembly_.get_mesh());

    auto elem_itr = filter.begin();
    auto elem_itr_e = filter.end();

    for(; elem_itr != elem_itr_e; ++elem_itr){

        auto* elem = *elem_itr;

        reinit(*elem);
        spatial_assembly_.assemble_one_element(mat, x, b);
        assemble_one_element(mat, x, b, old_solutions);
    }
}

template<int dim,typename SpatialAssembly>
void
SimpleFVTimeAssemblyPolicy<dim, SpatialAssembly>::assemble_one_element(TrilinosEpetraMatrix& mat, TrilinosEpetraVector& x,
                                                      TrilinosEpetraVector& b, const std::vector<TrilinosEpetraVector>& old_solutions ){

    auto dofs = spatial_assembly_.get_element_dofs(); //.size();
    auto elem_volume = spatial_assembly_.get_element_volume();

    std::vector<real_t> row_entries(dofs.size(), 0.0);

    mat.set_entry(dofs[0].id, dofs[0].id, elem_->volume()/dt_);
    auto old_sol = old_solutions[0][dofs[0].id];
    b.add(dofs[0].id, (old_sol*elem_volume)/dt_);
}
#endif

}

}

#endif // SIMPLE_FV_TIME_STEPPER_H
