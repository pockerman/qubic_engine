#include "kernel/maths/krylov_solvers/trilinos_krylov_solver.h"

#ifdef USE_TRILINOS

#include "kernel/base/kernel_consts.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"

namespace kernel{
namespace numerics{


void
TrilinosKrylovSolver::set_solver_data(PreconditionerType t, KrylovSolverType kst,
                                      uint_t n_itrs, real_t tol)
{
 set_preconditioner(t);
 set_krylov_solver(kst);
 n_itrs_ = n_itrs;
 tol_ = tol;
}


void
TrilinosKrylovSolver::set_preconditioner(PreconditionerType t)
{

    switch(t)
    {
    case PreconditionerType::JACOBI:
       linear_solver_.SetAztecOption(AZ_precond,AZ_Jacobi);
       prec_type_ = t;
      break;

    case PreconditionerType::ILU:
      linear_solver_.SetAztecOption(AZ_precond,AZ_ilu);
      prec_type_ = t;
      break;

    case PreconditionerType::ICC:
      linear_solver_.SetAztecOption(AZ_precond,AZ_icc);
      prec_type_ = t;
      break;

    case PreconditionerType::LU:
      linear_solver_.SetAztecOption(AZ_precond,AZ_lu);
      prec_type_ = t;
      break;

    default:
      linear_solver_.SetAztecOption(AZ_precond,AZ_ilu);
      prec_type_ = t;
    }
}


void
TrilinosKrylovSolver::set_krylov_solver(KrylovSolverType t)
{

  switch (t)
  {
    case KrylovSolverType::CG:
      linear_solver_.SetAztecOption(AZ_solver, AZ_cg); 
      kst_ = t;
      return;

    case KrylovSolverType::CGS:
      linear_solver_.SetAztecOption(AZ_solver, AZ_cgs); 
      kst_ = t;
      return;

    case KrylovSolverType::TFQMR:
      linear_solver_.SetAztecOption(AZ_solver, AZ_tfqmr); 
      kst_ = t;
      return;

    case KrylovSolverType::BICGSTAB:
      linear_solver_.SetAztecOption(AZ_solver, AZ_bicgstab); 
      kst_ = t;
      return;

    case KrylovSolverType::GMRES:
      linear_solver_.SetAztecOption(AZ_solver, AZ_gmres);
      kst_ = t; 
      return;

    default:
    {
       linear_solver_.SetAztecOption(AZ_solver, AZ_gmres);
       kst_ = t;
    }
      
  }
}


/*std::pair<uint_t,real_type>
TrilinosKrylovSolver::solve(const EpetraFEMat& A,
                                   EpetraVec& x,
                                   const EpetraVec& b)
{

   if(n_itrs_ == 0){
       n_itrs_ = A.m();
   }
   if(tol_ == biblsimpp_value_traits<real_type>::zero()) tol_ = biblSimPP::TOL();

   linear_solver_.SetAztecOption(AZ_max_iter,n_itrs_);
   linear_solver_.SetAztecParam(AZ_tol,tol_);

   Epetra_FECrsMatrix * emat = const_cast<Epetra_FECrsMatrix*>(A.trilinos_epetra_matrix());
   Epetra_Vector * esol = x.trilinos_epetra_vector();
   Epetra_Vector * erhs = const_cast<Epetra_Vector*>(b.trilinos_epetra_vector());

   linear_solver_.Iterate(emat, esol, erhs, n_itrs_, tol_);

  // return the # of its. and the final residual norm.
  return std::make_pair(linear_solver_.NumIters(), linear_solver_.TrueResidual());

}*/



std::pair<uint_t, real_t>
TrilinosKrylovSolver::solve(const TrilinosEpetraMatrix& A, TrilinosEpetraVector& x, const TrilinosEpetraVector& b)
{

   if(n_itrs_ == 0){

       n_itrs_ = A.m();
   }

   if(tol_ == std::numeric_limits<real_t>::max()){

       tol_ = KernelConsts::tolerance();
   }

   linear_solver_.SetAztecOption(AZ_max_iter,n_itrs_);
   linear_solver_.SetAztecParam(AZ_tol,tol_);

   Epetra_CrsMatrix * emat = const_cast<TrilinosEpetraMatrix&>(A).get_matrix();

   Epetra_Vector * esol = x.get_vector();
   Epetra_Vector * erhs = const_cast<TrilinosEpetraVector&>(b).get_vector(); //b.trilinos_epetra_vector());

   linear_solver_.Iterate(emat, esol, erhs, n_itrs_, tol_);

  // return the # of its. and the final residual norm.
  return std::make_pair(linear_solver_.NumIters(), linear_solver_.TrueResidual());

}

}

}
#endif
