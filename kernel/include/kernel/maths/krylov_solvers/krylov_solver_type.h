#ifndef KRYLOV_SOLVER_TYPE_H
#define KRYLOV_SOLVER_TYPE_H

namespace kernel{
namespace numerics {


 /// \brief useful enumeration of the Krylov subspace
 /// methods we support
 enum class KrylovSolverType{CG, GMRES,
                             BICGSTAB,CGS,
                             TFQMR,INVALID_SOLVER};

}
}

#endif // KRYLOV_SOLVER_TYPE_H
