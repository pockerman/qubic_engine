#ifndef TRILINOS_PISO_SOLVER_H
#define TRILINOS_PISO_SOLVER_H

#include "kernel/base/config.h"

#if defined (USE_FVM) && defined (USE_TRILINOS)

#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#include "kernel/numerics/krylov_solvers/krylov_solver_data.h"

namespace kernel{
namespace numerics{


///
/// \brief The TrilinosPISOSolver class
///
class TrilinosPISOSolver
{
public:

    ///
    /// \brief vector_t
    ///
    typedef  kernel::numerics::TrilinosEpetraVector vector_t;

    ///
    /// \brief matrix_t
    ///
    typedef  kernel::numerics::TrilinosEpetraMatrix matrix_t;

    ///
    /// \brief TrilinosPISOSolver
    ///
    TrilinosPISOSolver(const KrylovSolverData& velocity_data=KrylovSolverData(),
                       const KrylovSolverData& pressure_data=KrylovSolverData());

    ///
    /// \brief Solve the given Navier-Stokes systems
    ///
    template<typename NSSystem>
    void solve(NSSystem& sysytem);

private:

    ///
    /// \brief velocity_data_
    ///
    KrylovSolverData& velocity_data_;

    ///
    /// \brief pressure_data_
    ///
    KrylovSolverData& pressure_data_;

};

template<typename NSSystem>
void TrilinosPISOSolver::solve(NSSystem& sysytem){

}

}
}
#endif

#endif // TRILINOS_PISO_SOLVER_H
