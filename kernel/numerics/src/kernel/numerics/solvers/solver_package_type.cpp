#include "kernel/numerics/solvers/solver_package_type.h"

namespace kernel{
namespace maths{
namespace solvers{


std::string type_to_string(SolverPackageType type){

    switch(type){
    case SolverPackageType::TRI_AMESOS:
        return "TRI_AMESOS";
    case SolverPackageType::TRI_AZTECOO:
        return "TRI_AZTECOO";
    case SolverPackageType::BLAZE:
        return "BLAZE";
    }

    return "INVALID_TYPE";
}

}
}
}
