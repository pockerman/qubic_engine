#include "kernel/numerics/fv_gauss_grad.h"
#include "kernel/numerics/fv_grad_types.h"
#include "kernel/discretization/element.h"
#include "kernel/discretization/face_element.h"


namespace kernel{
namespace numerics {

template<int dim>
FVGaussGrad<dim>::FVGaussGrad()
    :
      FVGradBase<dim>(FVGradType::GAUSS)
{}

template<int dim>
void
FVGaussGrad<dim>::compute_gradients(const Element<dim>& elem, std::vector<real_t>& values)const{

    {
        std::vector<real_t> empty;
        values.swap(empty);
    }

    values.reserve(elem.n_faces());

    for(uint_t f=0; f<elem.n_faces(); ++f){

        auto& face = elem.get_face(f);
        values.push_back(face.volume()); //face.owner_neighbor_distance());
    }
}

template class FVGaussGrad<1>;
template class FVGaussGrad<2>;
template class FVGaussGrad<3>;

}

}
