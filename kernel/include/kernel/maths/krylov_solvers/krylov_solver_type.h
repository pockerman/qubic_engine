#ifndef KRYLOV_SOLVER_TYPE_H
#define KRYLOV_SOLVER_TYPE_H

#include <string>
namespace kernel{
namespace numerics {


 /// \brief useful enumeration of the Krylov subspace
 /// methods we support
 enum class KrylovSolverType{CG, GMRES,
                             BICGSTAB,CGS,
                             TFQMR,INVALID_SOLVER};


std::string krylov_solver_to_string(KrylovSolverType type);
}
}

#endif // KRYLOV_SOLVER_TYPE_H
