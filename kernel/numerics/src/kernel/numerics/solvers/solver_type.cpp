#include "kernel/numerics/solvers/solver_type.h"

namespace kernel {
namespace maths {
namespace solvers{

std::string solver_type_to_string(SolverType type){

    switch(type){
    case SolverType::DIRECT:
        return "DIRECT";
    case SolverType::ITERATIVE:
        return "ITERATIVE";
    }

    return "INVALID_TYPE";
}

}

}

}
