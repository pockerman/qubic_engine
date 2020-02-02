#include "kernel/numerics/dof_object.h"
#include "kernel/base/kernel_consts.h"

#include <algorithm>
#include <exception>

namespace kernel
{

void
DoF::invalidate_dof(DoF& dof){
    dof.id = KernelConsts::invalid_size_type();
    dof.var_name = KernelConsts::dummy_string();
    dof.active = false;
}

void
DoF::invalidate_dof(DoF&& dof){

    dof.id = KernelConsts::invalid_size_type();
    dof.var_name = KernelConsts::dummy_string();
    dof.active = false;
}

DoFObject::DoFObject()
    :
   dofs_()
{}

void
DoFObject::insert_dof(DoF&& dof){

    auto name = dof.var_name;

    if(has_variable(name)){
        throw std::logic_error("Dof already exists");
    }

    dofs_.push_back(dof);
    DoF::invalidate_dof(dof);

}

bool
DoFObject::has_variable(const std::string& variable)const{

    auto itr = std::find_if(dofs_.begin(), dofs_.end(),
                            [&](const DoF& dof){
        return dof.var_name == variable;
    });

    if(itr != dofs_.end()){
        return true;
    }

    return false;
}
}
