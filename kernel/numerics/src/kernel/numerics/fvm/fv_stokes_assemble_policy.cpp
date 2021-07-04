#include "kernel/numerics/fvm/fv_stokes_assemble_policy.h"

#ifdef USE_FVM

#include "kernel/numerics/fvm/fv_laplace_assemble_policy.h"

namespace kernel{
namespace numerics{
namespace fvm{

template<>
std::array<std::string, 2>
FVStokesAssemblyPolicy<2>::default_components(){
    return {"u", "v"};
}

template<>
std::array<std::string, 3>
FVStokesAssemblyPolicy<3>::default_components(){
    return {"u", "v", "w"};
}


template<int dim>
FVStokesAssemblyPolicy<dim>::FVStokesAssemblyPolicy(const std::array<std::string, dim>& components)
    :
      FVVectorAssemblePolicy<dim>(components)
{}

template<int dim>
void
FVStokesAssemblyPolicy<dim>::compute_fluxes(){

    // fix the gradient approximation
    auto& components = this->get_components();
    auto itr = components.begin();

    for(; itr != components.end(); ++itr){
        itr->second->compute_fluxes();
    }

}

template<int dim>
void
FVStokesAssemblyPolicy<dim>::reinit(const Element<dim>& element){

    // fix the gradient approximation
    auto& components = this->get_components();
    auto itr = components.begin();

    for(; itr != components.end(); ++itr){
        itr->second->reinit(element);
    }

}

template<int dim>
void
FVStokesAssemblyPolicy<dim>::reinit(const std::array<std::string, dim>& names){

    this->FVVectorAssemblePolicy<dim>::reinit(names);

    // fix the gradient approximation
    auto& components = this->get_components();

    auto itr = components.begin();

    for(; itr != components.end(); ++itr){
        itr->second = std::make_shared<FVLaplaceAssemblyPolicy<dim>>();
    }
}

template class FVStokesAssemblyPolicy<2>;
template class FVStokesAssemblyPolicy<3>;

}
}
}

#endif
