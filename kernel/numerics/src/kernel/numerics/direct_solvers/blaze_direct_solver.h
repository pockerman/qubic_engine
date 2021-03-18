#ifndef BLAZE_DIRECT_SOLVER_H
#define BLAZE_DIRECT_SOLVER_H

#include "kernel/numerics/direct_solvers/direct_solver_base.h"
#include "kernel/numerics/direct_solvers/direct_solver_type.h"

namespace kernel{
namespace maths {
namespace solvers {


///
/// \brief The AmesosDirectConfig struct. Helper struct
/// that wraps parameters to be passed to the AmesosDirect class
///
struct BlazeDirectSolverConfig
{

   DirectSolverType dstype;
};

class BlazeDirectSolver: public DirectSolverBase<DynMat<real_t>, DynVec<real_t>>
{
public:

    ///
    /// \brief matrix_t The matrix type the solver is using
    ///
    typedef typename DirectSolverBase<DynMat<real_t>, DynVec<real_t>>::matrix_t matrix_t;

    ///
    /// \brief vector_t The vector type the solver is using
    ///
    typedef typename DirectSolverBase<DynMat<real_t>, DynVec<real_t>>::vector_t vector_t;

    ///
    /// \brief BlazeDirectSolver Constructor
    ///
    BlazeDirectSolver(const BlazeDirectSolverConfig& config);

    ///
    /// \brief solve. Solve the system Ax=b
    ///
    virtual void solve(const matrix_t& A, vector_t& x, const vector_t& b )const override final;


private:

    ///
    /// \brief config_ Configuration of the solver
    ///
    BlazeDirectSolverConfig config_;

};

}

}
}

#endif // BLAZE_DIRECT_SOLVER_H
