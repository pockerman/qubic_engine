#ifndef TRILINOS_KRYLOV_SOLVER_H
#define TRILINOS_KRYLOV_SOLVER_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"
#include "kernel/maths/krylov_solvers/krylov_solver_output.h"
#include "kernel/maths/krylov_solvers/krylov_solver_data.h"

// Trilinos include files.
#include <AztecOO.h>

#include <limits>


namespace kernel{
namespace numerics{

/// forward declarations
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;

///
/// \brief Class that models a Krylov type solver
///
class TrilinosKrylovSolver
{

public:

  typedef KrylovSolverResult output_t;

    /// default ctor
    TrilinosKrylovSolver();

    /// \brief construct by passing in the preconditioner and solver types
    TrilinosKrylovSolver(const KrylovSolverData& data);

    /// \brief dtor
    ~TrilinosKrylovSolver();

    /// set the solver iterations
    void set_solver_iterations(uint_t nitrs){data_.n_iterations = nitrs;}

    /// set the solver tolerance
    void set_solver_tolerance(real_t tol){data_.tolerance = tol;}

    /// set the data of the solver
    void set_solver_data(const KrylovSolverData& data);

    /// \brief Solve the given system
    output_t solve(const TrilinosEpetraMatrix& A,
                 TrilinosEpetraVector& x, const TrilinosEpetraVector& b);

protected:

    /// \brief set the preconditioner that the solver uses
    void set_preconditioner();

    /// set the solver type we want to use
    void set_krylov_solver();
  
private:
  
  /// \brief the data the solver is using
  KrylovSolverData data_;
  
  /// \brief Iplement the Krylov solver by using Trilinos::AztecOO
  AztecOO  linear_solver_;

};

inline
TrilinosKrylovSolver::TrilinosKrylovSolver()
                            :
                            data_(),
                            linear_solver_()
                            {} 
                            

inline
TrilinosKrylovSolver::TrilinosKrylovSolver(const KrylovSolverData& data)
                            :
                           data_(data),
                           linear_solver_()
{
    set_preconditioner();
    set_krylov_solver();
}


inline
TrilinosKrylovSolver::~TrilinosKrylovSolver()
{}

}
}
#endif
#endif
