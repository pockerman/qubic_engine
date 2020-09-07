#include "kernel/maths/direct_solvers/amesos_solver.h"

#ifdef USE_TRILINOS

#include "kernel/maths/solvers/solver_package_type.h"
#include "kernel/maths/trilinos_epetra_multivector.h"

//#include <Teuchos_RCP.hpp>

#include "Amesos_ConfigDefs.h"
#include "Amesos.h"
#include "Epetra_LinearProblem.h"
#include <Epetra_Vector.h>

#include <stdexcept>
#include <memory>
#include <iostream>

namespace kernel {
namespace maths {
namespace solvers {

AmesosDirect::AmesosDirect(AmesosDirectConfig options)
    :
      DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                       kernel::maths::algebra::TrilinosEpetraMultiVector>(SolverPackageType::TRI_AMESOS),
      options_(options)
{}

void
AmesosDirect::solve(const AmesosDirect::matrix_t& A, AmesosDirect::vector_t& x,
                     const AmesosDirect::vector_t& b )const{

    std::string dstype = solver_type_to_string(options_.dstype);
    Amesos factory;

    if(dstype == "SUPER_LU"){
        dstype = "SuperLU";

        if( !factory.Query(dstype.c_str()) ){
           throw std::logic_error("Amesos2 has not been configured to use SuperLU ");
        }

        std::cout<<"Using "<<dstype<<" direct solver"<<std::endl;
    }
    else if(dstype == "LU"){
        dstype = "Klu";
        std::cout<<"Using "<<dstype<<" direct solver"<<std::endl;
    }
    else{
        throw std::logic_error("Invalid direct solver type: " + dstype);
    }

    auto* mat = A.get_matrix();

    if(!mat){
      throw std::logic_error("Epetra_CrsMatrix has not been created");
    }

    const auto* rhs = b.raw_trilinos_vec_ptr();

    if(!rhs){
      throw std::logic_error("Epetra_MultiVector  rhs has not been created");
    }


    auto* sol = x.raw_trilinos_vec_ptr();

    if(!sol){
      throw std::logic_error("Epetra_MultiVector solution has not been created");
    }

    auto*& vec_rhs = rhs->operator()(0);
    auto*& vec = sol->operator()(0);

    Epetra_LinearProblem Problem(const_cast<Epetra_CrsMatrix*>(mat), vec,
                                 const_cast<Epetra_MultiVector*>(rhs));

    // Create solver interface to the requested
    // solver with Amesos factory method
    std::unique_ptr<Amesos_BaseSolver> solver(factory.Create(dstype.c_str(), Problem));

    if(solver.get() == nullptr){
        throw std::logic_error("Amesos::Solver is not built");
    }

    Teuchos::ParameterList list;

    if(options_.print_timing){
      list.set("PrintTiming", true);
    }

    if(options_.print_status){
       list.set("PrintStatus", true);
    }

    if(list.numParams() != 0){
        solver->SetParameters(list);
    }

    if(options_.do_symbolic_factorization){
        solver->SymbolicFactorization();
    }

    if(options_.do_numeric_factorization){
        solver->NumericFactorization();
    }
    solver->Solve();
}

}
}
}

#endif
