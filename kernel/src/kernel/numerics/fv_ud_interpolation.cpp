#include "kernel/numerics/fv_ud_interpolation.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face_element.h"

#include <exception>

namespace kernel{
namespace numerics{


template<int dim>
FVUDInterpolate<dim>::FVUDInterpolate()
    :
    FVInterpolateBase<dim>(FVInterpolationType::UD),
    velocity_(nullptr)
{}

template<int dim>
FVUDInterpolate<dim>::FVUDInterpolate(const NumericVectorFunctionBase<dim>& velocity)
    :
    FVInterpolateBase<dim>(FVInterpolationType::UD),
    velocity_(&velocity)
{}

template<int dim>
FVUDInterpolate<dim>::~FVUDInterpolate()
{}

template<int dim>
void
FVUDInterpolate<dim>::compute_fluxes(const Element<dim>& elem, std::vector<real_t>& values)const{

    if(!velocity_){
        throw std::logic_error("Velocity pointer is NULL");
    }
}

template<int dim>
void
FVUDInterpolate<dim>::compute_matrix_contributions(const Element<dim>& element,
                                                   std::vector<real_t>& values)const{

    if(!velocity_){
        throw std::logic_error("Velocity pointer is NULL");
    }

    {
        std::vector<real_t> zero(values.size(), 0.0);
        values.swap(zero);
    }

    // loop over the element faces
    for(uint_t f=0; f<element.n_faces(); ++f){

        auto& face = element.get_face(f);

        if(!face.on_boundary()){

            auto flux = compute_flux(face);

            if(face.is_owner(element.get_id())){

                if(flux >= 0.0){

                    // the contribution goes to the
                    // diagonal
                    values[0] += flux;
                }
                else{
                    // it goes to the off diagonal
                    // which neighbor this is
                    auto& neighbor = face.get_neighbor();
                    auto idx = element.which_neighbor_am_i(neighbor);
                    values[idx] += flux;

                }
            }
            else
            {
               if(flux >= 0.0){

                   // it goes to the off diagonal
                   // which neighbor this is
                   auto& neighbor = face.get_neighbor();
                   auto idx = element.which_neighbor_am_i(neighbor);
                   values[idx] -= flux;

               }
             else{
                   // the contribution goes to the
                   // diagonal
                   values[0] -= flux;
               }
            }
        }
    }
}

template class FVUDInterpolate<1>;
template class FVUDInterpolate<2>;
template class FVUDInterpolate<3>;
}
}
