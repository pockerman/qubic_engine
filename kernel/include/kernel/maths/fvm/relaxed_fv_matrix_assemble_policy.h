#ifndef RELAXED_FV_MATRIX_ASSEMBLE_POLICY_H
#define RELAXED_FV_MATRIX_ASSEMBLE_POLICY_H

#include "kernel/base/types.h"
#include "kernel/discretization/element_mesh_iterator.h"
#include "kernel/discretization/mesh_predicates.h"
#include "kernel/numerics/dof.h"

#include "boost/noncopyable.hpp"

#include <map>
#include <vector>
#include <string>

namespace kernel{
namespace numerics {

template<typename MatrixPolicy>
class RelaxedFVMatrixAssemblyPolicy: private boost::noncopyable
{
public:

    typedef MatrixPolicy matrix_policy_t;
    typedef typename matrix_policy_t::matrix_t matrix_t;
    typedef typename matrix_policy_t::vector_t vector_t;
    typedef typename matrix_policy_t::solver_t solver_t;
    typedef typename matrix_policy_t::solver_output_t solver_output_t;

    /// \brief Constructor
    RelaxedFVMatrixAssemblyPolicy();

    /// \brief Returns the size of the diagonal
    uint_t diagonal_size()const{return D_.size();}

    /// \brief Returns the size of the upper diagonal
    uint_t upper_diagonal_size()const{return upperD_.size();}

    /// \brief Returns the size of the lower diagonal
    uint_t lower_diagonal_size()const{return lowerD_.size();}

    /// \brief Returns the diagonal entry associated with
    /// a given element id. It does not incorporate
    /// the effect of bc
    real_t diagonal_val(uint_t dof)const{return D_.get(dof);}

    /// \brief Returns  the diagonal for the given dof by incorporating the
    ///effect of the BC for the  side with the given id
    real_t diagonal_val_with_bc(uint_t dof, uint_t side_id)const;

    /// \brief Returns the boundary entry
    /// that should be added to the diagonal entry
    real_t get_boundary_entry(uint_t side_id)const;

    /// \brief Returns the boundary entry that should be
    /// added to the rhs
    real_t get_boundary_entry_rhs(uint_t side_id)const;

    /// \brief Initialize
    void init(uint_t dofs, uint_t nsides);

    /// \brief Zero the entries
    void zero();

    /// \brief Zero the diagonal
    void zero_diagonal(){D_.zero();}

    ///\brief Zero the upper off diagonal
    void zero_upper_diagonal(){upperD_.zero();}

    ///\brief Zero the lower off diagonal
    void zero_lower_diagonal(){lowerD_.zero();}

    /// \brief Set the i-th diagonal entry
    void set_diagonal_entry(uint_t i, real_t val){D_.set(i, val);}

    /// \brief Add to the i-th diagonal entry the value
    void add_to_diagonal_entry(uint_t i, real_t val){D_.add(i, val);}

    /// \brief Set the i-th diagonal entry
    void set_upper_diagonal_entry(uint_t i, real_t val){upperD_.set(i, val);}

    /// \brief Add to the i-th diagonal entry the value
    void add_to_upper_diagonal_entry(uint_t i, real_t val){upperD_.add(i, val);}

    /// \brief Set the i-th diagonal entry
    void set_lower_diagonal_entry(uint_t i, real_t val){lowerD_.set(i, val);}

    /// \brief Add to the i-th diagonal entry the value
    void add_to_lower_diagonal_entry(uint_t i, real_t val){lowerD_.add(i, val);}

    /// \brief Relax
    template<typename MeshTp>
    void relax(const MeshTp& mesh, const vector_t& old_solution);

    /// \brief Calculates the sum-mag of the off diagonal entries
    /// for the interior sides
    void sum_mag_off_diag(std::vector<real_t> &sum_Off_diag)const;

private:

    /// \brief The diagonal coefficients of the matrix
    vector_t D_;

    /// \brief The upper diagonal entries
    vector_t upperD_;

    /// \brief The lower diagonal entries
    vector_t lowerD_;

    /// \brief The rhs
    vector_t rhs_;

    /// field containing pseudo-matrix coeffs
    /// for internal cells. We hold here the coefficients
    /// for boundary sides. They key is the Side global id
    std::map<uint_t, real_t> internal_coeffs_;

    /// field containing pseudo-matrix coeffs
    /// for boundary cells. The key is the Side global id
    std::map<uint_t, real_t> boundary_coeffs_;

    /// \brief The relaxation coefficient
    real_t relax_factor_;

    /// \brief The name of the variable the matrix belongs
    std::string var_name_;

};

template<typename MatrixPolicy>
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::RelaxedFVMatrixAssemblyPolicy()
    :
      D_(),
      upperD_(),
      lowerD_(),
      internal_coeffs_(),
      boundary_coeffs_()
{}

template<typename MatrixPolicy>
void
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::init(uint_t ndofs, uint_t nsides){

    D_.init(ndofs, false);
    upperD_.init(nsides, false);
    lowerD_.init(nsides, false);
}

template<typename MatrixPolicy>
void
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::zero(){

    zero_diagonal();
    zero_upper_diagonal();
    zero_lower_diagonal();
}

template<typename MatrixPolicy>
real_t
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::diagonal_val_with_bc(uint_t dof, uint_t side_id)const{

    real_t val = diagonal_val(dof);
    val += get_boundary_entry(side_id);
    return val;
}

template<typename MatrixPolicy>
real_t
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::get_boundary_entry(uint_t side_id)const{

    auto itr = internal_coeffs_.find(side_id);

    if(itr == internal_coeffs_.end()){
        throw std::logic_error("Side id: " + std::to_string(side_id) + " does not exist");
    }

    return itr->second;
}

template<typename MatrixPolicy>
real_t
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::get_boundary_entry_rhs(uint_t side_id)const{

     auto itr = boundary_coeffs_.find(side_id);

     if(itr == boundary_coeffs_.end()){
         throw std::logic_error("Side id: " + std::to_string(side_id) + " does not exist");
     }

     return itr->second;
}

template<typename MatrixPolicy>
template<typename MeshTp>
void
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::relax(const MeshTp& mesh, const vector_t& old_solution){

    std::vector<real_t> sum_off(D_.size(), 0.0);

    sum_mag_off_diag(sum_off,0);

    std::vector<real_t> internal_coeffs;

    std::vector<DoF> dofs;

    ConstElementMeshIterator<Active, MeshTp> filter(mesh);

    auto begin = filter.begin();
    auto end = filter.end();

    for(; begin != end; ++begin){

          auto* cell = *begin;

          cell->get_dofs(var_name_, dofs);
          real_t diag_0 = D_.get(dofs[0].id);

           for(uint_t f=0; f<cell->n_faces(); ++f){

               auto& face = cell->get_face(f);

               if(face.on_boundary()){

                   get_internal_coefficients(face,internal_coeffs);
                   D_.add(dofs[0].id,
                                    (get_compt_mag(get_compt_minimum(internal_coeffs))
                                           -get_compt_minimum(internal_coeffs)));

                   sum_off[dofs[0].id] += (get_compt_mag(get_compt_minimum(internal_coeffs))-
                                           get_compt_minimum(internal_coeffs));
               }
           }

           //make sure that the matrix remains diagonally dominant
           D_.set(dofs[0].id,std::max(std::fabs(D_.get(dofs[0].id)),sum_off[dofs[0].id]));

           //then relax
           D_.set(dofs[0].id, D_.get(dofs[0].id)/relax_factor_);

           const real_t old_sol = old_solution.get(dofs[0].id);
           rhs_.add(dofs[0].id, (D_.get(dofs[0].id) - diag_0)*old_sol);
       }

}

template<typename MatrixPolicy>
void
RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::sum_mag_off_diag(std::vector<real_t> &sum_Off_diag)const


}
}

#endif // RELAXED_FV_MATRIX_ASSEMBLE_POLICY_H
