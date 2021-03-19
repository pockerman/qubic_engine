#include "kernel/numerics/krylov_solvers/krylov_solver_output.h"

namespace kernel{
namespace numerics{

std::ostream&
KrylovSolverResult::print(std::ostream& out)const{

    out<<"Solver: "<<krylov_solver_to_string(solver_type)<<std::endl;
    out<<"Preconditioner: "<<preconditioner_to_string(precondioner_type)<<std::endl;
    this->AlgInfo::print(out);
}

}
}
