#ifndef AMESOS2_SOLVER_H
#define AMESOS2_SOLVER_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/maths/direct_solvers/direct_solver_base.h"
#include "kernel/maths/direct_solvers/direct_solver_type.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
//#include "kernel/maths/t

namespace kernel {
namespace algebra{
class TrilinosEpetraMultiVector;
}

namespace numerics{
class TrilinosEpetraMatrix;
}

namespace maths {
namespace solvers {


class Amesos2Direct: public DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                                             kernel::algebra::TrilinosEpetraMultiVector>
{

public:

    ///
    /// \brief Amesos2Direct Constructor
    ///
    Amesos2Direct(DirectSolverType dstype);

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef typename DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                                      kernel::algebra::TrilinosEpetraMultiVector>::matrix_t matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef typename DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                                      kernel::algebra::TrilinosEpetraMultiVector>::vector_t vector_t;


    ///
    /// \brief solve. Solve the system Ax=b
    ///
    virtual void solve(const matrix_t& A, vector_t& x, const vector_t& b )override final;

private:

    ///
    /// \brief dstype_ The type of the direct solver to use
    /// This is converted internally so that Amesos2 package understands it
    ///
    DirectSolverType dstype_;
};

}

}

}

#endif

#endif // AMESOS2_SOLVER_H
