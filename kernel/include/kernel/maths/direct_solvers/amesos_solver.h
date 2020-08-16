#ifndef AMESOS_SOLVER_H
#define AMESOS_SOLVER_H

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

///
/// \brief The AmesosDirectOptions struct. Helper struct
/// that wraps parameters to be passed to the AmesosDirect class
///
struct AmesosDirectOptions
{

   DirectSolverType dstype;
   bool print_timing{true};
   bool print_status{true};
   bool do_symbolic_factorization{true};
   bool do_numeric_factorization{true};

};

class AmesosDirect: public DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                                             kernel::algebra::TrilinosEpetraMultiVector>
{

public:

    ///
    /// \brief Amesos2Direct Constructor
    ///
    AmesosDirect(AmesosDirectOptions options);

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
    virtual void solve(matrix_t& A, vector_t& x, vector_t& b )override final;


private:

    ///
    /// \brief options_ Options to configure the solver
    ///
    AmesosDirectOptions options_;
};

}

}

}

#endif

#endif // AMESOS2_SOLVER_H
