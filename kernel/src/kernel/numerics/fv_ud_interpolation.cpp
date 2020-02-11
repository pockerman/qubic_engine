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
FVUDInterpolate<dim>::compute_fluxes(const Element<dim>& element, std::vector<real_t>& values)const{

    if(!velocity_){
        throw std::logic_error("Velocity pointer is NULL");
    }

    {
        std::vector<real_t> zero(element.n_faces(), 0.0);
        values.swap(zero);
    }

    for(uint_t f=0; f<element.n_faces(); ++f){
        auto& face = element.get_face(f);
        values[f] = compute_flux(face);
    }
}

template<int dim>
void
FVUDInterpolate<dim>::compute_matrix_contributions(const Element<dim>& element,
                                                    std::map<uint_t, real_t>& values)const{

    if(!velocity_){
        throw std::logic_error("Velocity pointer is NULL");
    }

    /*{
        std::vector<real_t> zero(values.size(), 0.0);
        values.swap(zero);
    }*/

    // loop over the element faces
    for(uint_t f=0; f<element.n_faces(); ++f){

        auto& face = element.get_face(f);

        if(!face.on_boundary()){

            auto flux = compute_flux(face);

            if(face.is_owner(element.get_id())){

                if(flux >= 0.0){

                    // the contribution goes to the
                    // diagonal
                    values[element.get_id()] += flux;
                }
                else{
                    // it goes to the off diagonal
                    auto& neighbor = face.get_neighbor();

                    // which neighbor this is
                    auto idx = element.which_neighbor_am_i(neighbor);
                    values[neighbor.get_id()] += flux;
                }
            }
            else{

               if(flux >= 0.0){

                   // it goes to the off diagonal
                   auto& neighbor = face.get_owner();

                   // which neighbor this is
                   auto idx = element.which_neighbor_am_i(neighbor);

                   /*if(idx == KernelConsts::invalid_size_type()){
                       throw std::logic_error("Invalid neighbor index computed");
                   }

                   if(idx >= values.size()){
                        throw std::logic_error("Neighbor index computed does not fit fluxes container " +
                                               std::to_string(idx) +
                                               " not in [0, " +
                                               std::to_string(values.size())+
                                               ")");
                    }*/

                   values[neighbor.get_id()] -= flux;

               }
             else{
                   // the contribution goes to the
                   // diagonal
                   values[element.get_id()] -= flux;
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
