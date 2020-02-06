#ifndef TRILINOS_KRYLOV_SOLVER_H
#define TRILINOS_KRYLOV_SOLVER_H

#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/krylov_solvers/krylov_solver_type.h"
#include "kernel/maths/krylov_solvers/preconditioner_type.h"

// Trilinos include files.
#include <AztecOO.h>

#include <limits>


namespace kernel{
namespace numerics{

/// forward declarations
class TrilinosEpetraMatrix;
class TrilinosEpetraVector;


class TrilinosKrylovSolver
{

public:

  /// default ctor
  TrilinosKrylovSolver();
  
  ///
  /// \brief construct by passing in the preconditioner and solver types
  ///
  TrilinosKrylovSolver(PreconditionerType t,  KrylovSolverType kst,
                        uint_t n_itrs, real_t tol);
  
  /// \brief dtor
  ~TrilinosKrylovSolver();
  
  /// \brief set the preconditioner that the solver uses
  void set_preconditioner(PreconditionerType t);
  
  /// set the solver type we want to use
  void set_krylov_solver(KrylovSolverType t);

  /// set the solver iterations
  void set_solver_iterations(uint_t nitrs){n_itrs_ = nitrs;}

  /// set the solver tolerance
  ///
  void set_solver_tolerance(real_t tol){tol_ = tol;}

  /// set the data of the solver
  void set_solver_data(PreconditionerType t, KrylovSolverType kst, uint_t n_itrs, real_t tol);
  
  /// \brief Solve the given system
  std::pair<uint_t,real_t> solve(const TrilinosEpetraMatrix& A,
                                       TrilinosEpetraVector& x,
                                       const TrilinosEpetraVector& b);
  
private:
  
  /// \brief the preconditioner type we use
  PreconditionerType prec_type_;
  
  /// \brief the solver type we use
  KrylovSolverType kst_;

  /// \brief number of iterations that the iterative solver is using
  uint_t n_itrs_;

  /// \brief the tolerance the solver is using
  real_t tol_;
  
  /// \brief Iplement the Krylov solver by using Trilinos::AztecOO
  AztecOO  linear_solver_;

};

//inline members

inline
TrilinosKrylovSolver::TrilinosKrylovSolver()
                            :
                            prec_type_(PreconditionerType::INVALID_PREC),
                            kst_(KrylovSolverType::INVALID_SOLVER),
                            n_itrs_(0),
                            tol_(std::numeric_limits<real_t>::max()),
                            linear_solver_()
                            {} 
                            

inline
TrilinosKrylovSolver::TrilinosKrylovSolver(PreconditionerType t, KrylovSolverType kst,
                                          uint_t n_itrs,  real_t tol)
                            :
                            prec_type_(t),
                            kst_(kst),
                            n_itrs_(n_itrs),
                            tol_(tol),
                            linear_solver_()
{
  set_preconditioner(t);
  set_krylov_solver(kst);
}


inline
TrilinosKrylovSolver::~TrilinosKrylovSolver()
{}

}
}
#endif
#endif
