#ifndef FV_UD_INTERPOLATION_H
#define FV_UD_INTERPOLATION_H

#include "kernel/numerics/fvm/fv_interpolate_base.h"
#include "kernel/maths/functions/numeric_vector_function.h"

namespace kernel{
namespace numerics{

template<int dim> class NumericVectorFunctionBase;

template<int dim>
class FVUDInterpolate: public FVInterpolateBase<dim>
{

public:

    /// \brief Constructor
    FVUDInterpolate();

    /// \brief Constructor
    FVUDInterpolate(const NumericVectorFunctionBase<dim>& velocity);

    /// \brief destructor
    virtual ~FVUDInterpolate();

    /// \brief Compute the fluxes for the given element
    virtual void compute_fluxes(const Element<dim>& elem, std::vector<real_t>& values)const override;

    /// \brief Returns the matrix contibutions
    /// that should be used on the given element
    virtual void compute_matrix_contributions(const Element<dim>& element,  std::map<uint_t, real_t>& values)const override;

    /// \brief Compute the dot product of the
    /// velocity computed on the given face and
    /// the face normal vector
    template<typename FaceTp>
    real_t compute_flux(const FaceTp& face)const;


    /// \brief Set the object that calculates the velocity
    /// field needed for the calculation of fluxes
    void set_velocity(const NumericVectorFunctionBase<dim>& vel){velocity_  =&vel;}

protected:


    /// \brief Pointer to the object describing the boundary
    /// conditions
    const NumericVectorFunctionBase<dim>* velocity_;

};

template<int dim>
template<typename FaceTp>
real_t
FVUDInterpolate<dim>::compute_flux(const FaceTp& face)const{

    if(!velocity_){
        throw std::logic_error("Velocity pointer is NULL");
    }

    const DynVec<real_t> velocity_value = velocity_->value(face.centroid());

    return dot(velocity_value, face.normal_vector());
}

}
}

#endif // FV_UD_INTERPOLATION_H
