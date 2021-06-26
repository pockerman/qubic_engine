#include "kernel/numerics/fvm/fv_vector_assemble_policy.h"

#ifdef USE_FVM

namespace kernel{
namespace numerics {


namespace fvm{

template<int dim>
FVVectorAssemblePolicy<dim>::FVVectorAssemblePolicy(const std::array<std::string, dim>& components)
    :
      comp_assmble_policies_()
{
    reinit(components);
}

template<int dim>
void
FVVectorAssemblePolicy<dim>::reinit(const std::array<std::string, dim>& components){

    comp_assmble_policies_.clear();

    for(const auto& name:components){
        comp_assmble_policies_.emplace(name, std::shared_ptr<FVAssemblyPolicy<dim>>());
    }

}

template<int dim>
void
FVVectorAssemblePolicy<dim>::set_mesh(const Mesh<dim>& mesh){

#ifdef KERNEL_DEBUG
    assert(!comp_assmble_policies_.empty() && "Empty components");
#endif

    auto itr = comp_assmble_policies_.begin();
    for(; itr != comp_assmble_policies_.end(); ++itr){
        itr->second->set_mesh(mesh);
    }


    //std::for_each(comp_assmble_policies_.begin(), comp_assmble_policies_.end(),
    //              [&mesh](std::shared_ptr<FVAssemblyPolicy<dim>> policy){
    //    policy->set_mesh(mesh);});

}

template<int dim>
void
FVVectorAssemblePolicy<dim>::reinit(const Element<dim>& element){

#ifdef KERNEL_DEBUG
    assert(!comp_assmble_policies_.empty() && "Empty components");
#endif

    auto itr = comp_assmble_policies_.begin();
    for(; itr != comp_assmble_policies_.end(); ++itr){
        itr->second->reinit(element);
    }

    //std::for_each(comp_assmble_policies_.begin(), comp_assmble_policies_.end(),
    //              [&element](std::shared_ptr<FVAssemblyPolicy<dim>> policy){
    //    policy->reinit(element);});

}

template class FVVectorAssemblePolicy<2>;
template class FVVectorAssemblePolicy<3>;
}
}
}
#endif
