#ifndef BLAZE_KRYLOV_SOLVER_H
#define BLAZE_KRYLOV_SOLVER_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"
#include "kernel/maths/krylov_solvers/krylov_solver_output.h"
#include "kernel/maths/krylov_solvers/krylov_solver_data.h"

#include <memory>

namespace kernel {
namespace numerics{

/// Forward declarations
class BlazeKrylovSolverImplBase;


/// \brief Class that models a Krylov type solver
/// usinf Blaze local data structures
class BlazeKrylovSolver
{

public:

    /// \brief The output the solver returns when solving
    /// the system
    typedef KrylovSolverResult output_t;

    /// default ctor
    BlazeKrylovSolver();

    /// \brief construct by passing in the preconditioner and solver types
    BlazeKrylovSolver(const KrylovSolverData& data);

    /// \brief dtor
    ~BlazeKrylovSolver();

    /// set the solver iterations
    void set_solver_iterations(uint_t nitrs){data_.n_iterations = nitrs;}

    /// set the solver tolerance
    void set_solver_tolerance(real_t tol){data_.tolerance = tol;}

    /// set the data of the solver
    void set_solver_data(const KrylovSolverData& data);

    /// \brief Solve the given system
    output_t solve(const SparseMatrix<real_t>& A,
                   DynVec<real_t>& x, const DynVec<real_t>& b);

protected:

    /// \brief set the preconditioner that the solver uses
    void set_preconditioner();

    /// set the solver type we want to use
    void set_krylov_solver();

private:

  /// \brief the data the solver is using
  KrylovSolverData data_;

  /// \brief Iplement the Krylov solver by using
  /// local implementation of iterative solvers
  std::unique_ptr<BlazeKrylovSolverImplBase> pimpl_;

};

inline
BlazeKrylovSolver::BlazeKrylovSolver()
                            :
                            data_(),
                            pimpl_()
                            {}


inline
BlazeKrylovSolver::BlazeKrylovSolver(const KrylovSolverData& data)
                            :
                           data_(data),
                           pimpl_()
{
    set_preconditioner();
    set_krylov_solver();
}


inline
BlazeKrylovSolver::~BlazeKrylovSolver()
{}

}

}

#endif // BLAZE_KRYLOV_SOLVER_H
