#include "kernel/numerics/krylov_solvers/trilinos_krylov_solver.h"

#ifdef USE_TRILINOS

#include "kernel/base/kernel_consts.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#include <chrono>

namespace kernel{
namespace numerics{

void
TrilinosKrylovSolver::set_solver_data(const KrylovSolverData& data){
 data_ = data;
 set_preconditioner();
 set_krylov_solver();
}


void
TrilinosKrylovSolver::set_preconditioner()
{

    switch(data_.precondioner_type)
    {
    case PreconditionerType::JACOBI:
       linear_solver_.SetAztecOption(AZ_precond,AZ_Jacobi);
       break;

    case PreconditionerType::ILU:
      linear_solver_.SetAztecOption(AZ_precond, AZ_dom_decomp);
      linear_solver_.SetAztecOption(AZ_subdomain_solve, AZ_ilu);
      break;

    case PreconditionerType::ICC:
      linear_solver_.SetAztecOption(AZ_precond,AZ_icc);
      break;

    case PreconditionerType::LU:
        linear_solver_.SetAztecOption(AZ_precond, AZ_dom_decomp);
        linear_solver_.SetAztecOption(AZ_subdomain_solve, AZ_lu);
      break;

    default:
        linear_solver_.SetAztecOption(AZ_precond, AZ_dom_decomp);
        linear_solver_.SetAztecOption(AZ_subdomain_solve, AZ_ilu);
    }
}


void
TrilinosKrylovSolver::set_krylov_solver(){

  switch (data_.solver_type)
  {
    case KrylovSolverType::CG:
      linear_solver_.SetAztecOption(AZ_solver, AZ_cg); 
      return;

    case KrylovSolverType::CGS:
      linear_solver_.SetAztecOption(AZ_solver, AZ_cgs); 
      return;

    case KrylovSolverType::TFQMR:
      linear_solver_.SetAztecOption(AZ_solver, AZ_tfqmr); 
      return;

    case KrylovSolverType::BICGSTAB:
      linear_solver_.SetAztecOption(AZ_solver, AZ_bicgstab); 
      return;

    case KrylovSolverType::GMRES:
      linear_solver_.SetAztecOption(AZ_solver, AZ_gmres);
      return;

    default:
    {
       linear_solver_.SetAztecOption(AZ_solver, AZ_gmres);
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



TrilinosKrylovSolver::output_t
TrilinosKrylovSolver::solve(const TrilinosEpetraMatrix& A, TrilinosEpetraVector& x, const TrilinosEpetraVector& b){


   std::chrono::time_point<std::chrono::system_clock> start, end;
   start = std::chrono::system_clock::now();
   KrylovSolverResult result;

   result.nprocs = 1;
   result.nthreads = 1;
   result.solver_type = data_.solver_type;
   result.precondioner_type = data_.precondioner_type;

   if(data_.n_iterations == 0){
       data_.n_iterations = A.m();
   }

   if(data_.tolerance == std::numeric_limits<real_t>::max()){
       data_.tolerance = KernelConsts::tolerance();
   }

   linear_solver_.SetAztecOption(AZ_max_iter, data_.n_iterations);
   linear_solver_.SetAztecParam(AZ_tol, data_.tolerance);

   Epetra_CrsMatrix * emat = const_cast<TrilinosEpetraMatrix&>(A).get_matrix();

   Epetra_Vector * esol = x.get_vector();
   Epetra_Vector * erhs = const_cast<TrilinosEpetraVector&>(b).get_vector();

   linear_solver_.Iterate(emat, esol, erhs, data_.n_iterations, data_.tolerance);

   end = std::chrono::system_clock::now();

   result.runtime = end-start;
   result.niterations = linear_solver_.NumIters();
   result.residual = linear_solver_.TrueResidual();


  // return the # of its. and the final residual norm.
  return result;

}

}

}
#endif
