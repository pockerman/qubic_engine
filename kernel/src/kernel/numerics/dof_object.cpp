#include "kernel/numerics/dof_object.h"
#include "kernel/base/kernel_consts.h"

#include <algorithm>
#include <exception>

namespace kernel{
namespace numerics {


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

    dofs_.insert_or_assign(dof.var_name, dof);
    DoF::invalidate_dof(dof);
}

bool
DoFObject::has_variable(std::string_view variable)const{
    return dofs_.find(variable) != dofs_.end();
}

void
DoFObject::invalidate_dofs(std::string_view name){
    auto itr = dofs_.find(name);

    if(itr != dofs_.end()){
            DoF::invalidate_dof(itr->second);
    }
}

DoF
DoFObject::get_dof(std::string_view name)const{
    auto itr = dofs_.find(name);

    if(itr != dofs_.end()){
        return itr->second;
    }

    return DoF();
}
}

}
