#ifndef DOF_H
#define DOF_H

#include "kernel/base/types.h"
#include <string>

namespace kernel {
namespace numerics {

struct DoF
{
    std::string_view var_name;
    uint_t id;
    bool active;

    static void invalidate_dof(DoF& dof);
    static void invalidate_dof(DoF&& dof);
};

}

}

#endif // DOF_H
