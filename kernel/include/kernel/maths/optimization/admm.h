#ifndef ADMM_H
#define ADMM_H

#include "kernel/base/types.h"
#include "kernel/maths/solvers/solver_base.h"

namespace kernel{
namespace maths {
namespace opt{

///
/// \brief The ADMM class. Implements the ADMM algorithm for solving the
/// convex quadratic problem
/// \f[ \text{minimize}~~ \frac{1}{2}x^TPx +q^Tx \f]
///
/// subject to
///
/// \f[ l \leq Ax \leq u \f]
///

template<typename MatrixTp, typename VectorTp>
class ADMM
{

public:

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef MatrixTp matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef MatrixTp vector_t;

    ///
    /// \brief ADMM Constructor
    ///
    ADMM(kernel::maths::solvers::SolverBase<matrix_t, vector_t>& solver);

    ///
    /// \brief solve
    ///
    void solve();

private:

    ///
    /// \brief rho_. The step size of the algorithm
    ///
    real_t rho_;

    ///
    /// \brief solver_ The solver to use to solve the system
    ///
    kernel::maths::solvers::SolverBase<matrix_t, vector_t>* solver_ptr_;

};

}
}
}

#endif // ADMM_H
