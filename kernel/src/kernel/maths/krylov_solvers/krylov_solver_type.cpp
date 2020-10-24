#include "kernel/maths/krylov_solvers/krylov_solver_type.h"

namespace kernel {
namespace numerics {

std::string krylov_solver_to_string(KrylovSolverType type){
    switch(type){
    case KrylovSolverType::CG:
        return "CG";
    case KrylovSolverType::CGS:
        return "CGS";
    case KrylovSolverType::GMRES:
        return "GMRES";
    case KrylovSolverType::TFQMR:
        return "TFQMR";
    case KrylovSolverType::BICGSTAB:
        return "BICGSTAB";
    }

    return "INVALID_SOLVER";
}

}

}
