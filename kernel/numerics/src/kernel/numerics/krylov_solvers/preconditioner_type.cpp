#include "kernel/numerics/krylov_solvers/preconditioner_type.h"

namespace kernel{
namespace numerics {

std::string preconditioner_to_string(PreconditionerType type){
    switch(type){
    case PreconditionerType::LU:
        return "LU";
    case PreconditionerType::ICC:
        return "ICC";
    case PreconditionerType::ILU:
        return "ILU";
    case PreconditionerType::JACOBI:
        return "JACOBI";
    }

    return "INVALID_PREC";
}

}
}
