#include "kernel/numerics/dof.h"
#include "kernel/base/kernel_consts.h"

namespace kernel {
namespace numerics {

void
DoF::invalidate_dof(DoF& dof){
    dof.id = KernelConsts::invalid_size_type();
}

void
DoF::invalidate_dof(DoF&& dof){

    dof.id = KernelConsts::invalid_size_type();
    dof.var_name = KernelConsts::dummy_string();
    dof.active = false;
}

}

}
