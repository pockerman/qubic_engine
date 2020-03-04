#ifndef FV_VECTOR_MATRIX_H
#define FV_VECTOR_MATRIX_H

#include "kernel/maths/fvm/relaxed_fv_matrix_assemble_policy.h"

#include <map>
#include <string>

namespace kernel{
namespace numerics{

/// forward declarations
template<typename AssemblyPolicy> class FVVectorMatrix;


/// \brief Wrapper to
template<typename MatrixPolicy>
class FVVectorMatrix<RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>>
{
public:

    typedef MatrixPolicy matrix_policy_t;
    typedef typename matrix_policy_t::matrix_t matrix_t;
    typedef typename matrix_policy_t::vector_t vector_t;
    typedef typename matrix_policy_t::solver_t solver_t;
    typedef typename matrix_policy_t::solver_output_t solver_output_t;

    using RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::relax;
    using RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>::init;

    /// \brief Add matrix
    void add_matrix(const std::string& name);

    /// \brief Returns the number of matrices
    uint_t n_mats()const{return matrices_.size();}

    /// \brief Returns the central coefficient of the matrix
    /// scaled by the volume of the element
    template<typename ElementType>
    real_t A(const std::string& name, const ElementType& element)const;

private:

    std::map<std::string, RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>> matrices_;

};

template<typename MatrixPolicy>
template<typename ElementType>
real_t
FVVectorMatrix<RelaxedFVMatrixAssemblyPolicy<MatrixPolicy>>::A(const std::string& name,
                                                               const ElementType& element)const{

       double diag = scalar_mats_[0]->D(dof_index);

       auto itr = matrices_.find(name);

       if(itr == matrices_.end()){
           throw std::logic_error("Matrix: " + name + " does not exist" );
       }

       real_t vol = element.volume();

       std::vector<const Side<dim>* > active_sides;

       dof_cell->geom_object()->fill_active_sides_family_tree(active_sides);

       fvmcpp_assert(active_sides.empty() != true,"\nEmpty Active Sides\n");

       const unsigned int as_size = active_sides.size();

       for(unsigned int as = 0; as<as_size; as++)
       {

         if(active_sides[as]->is_side_on_boundary()==true)
         {

           this->addCmptAvBoundaryDiag(diag,*active_sides[as]);

         }
      }

      return diag/vol;

}

}
}

#endif // FV_VECTOR_MATRIX_H
