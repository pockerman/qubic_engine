#include "kernel/maths/direct_solvers/direct_solver_type.h"

namespace kernel {
namespace maths{
namespace solvers{


std::string solver_type_to_string(DirectSolverType type){

    switch(type){
    case DirectSolverType::SUPER_LU:
        return "SUPER_LU";
    }

    return "INVALID_TYPE";
}

}
}
}
