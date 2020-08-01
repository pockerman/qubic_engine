#include "kernel/maths/solvers/solver_package_type.h"

namespace kernel{
namespace maths{
namespace solvers{


std::string solver_package_type_to_string(SolverPackageType type){
    switch(type){
    case SolverPackageType::TRI_AMESOS2:
        return "TRI_AMESOS2";
    case SolverPackageType::TRI_AZTECOO:
        return "TRI_AZTECOO";
    }

    return "INVALID_TYPE";
}

}
}
}
