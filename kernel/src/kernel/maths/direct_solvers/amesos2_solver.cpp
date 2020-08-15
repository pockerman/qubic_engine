#include "kernel/maths/direct_solvers/amesos2_solver.h"

#ifdef USE_TRILINOS

#include "kernel/maths/solvers/solver_package_type.h"
#include "kernel/maths/trilinos_epetra_multivector.h"

#include <Teuchos_RCP.hpp>
//#include <Teuchos_ScalarTraits.hpp>
//#include <Teuchos_oblackholestream.hpp>
//#include <Teuchos_Tuple.hpp>
//#include <Teuchos_VerboseObject.hpp>

//#include <Amesos2.hpp>
//#include "Amesos2_Version.hpp"

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

Amesos2Direct::Amesos2Direct(DirectSolverType dstype)
    :
      DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                       kernel::algebra::TrilinosEpetraMultiVector>(SolverPackageType::TRI_AMESOS2),
      dstype_(dstype)
{}

void
Amesos2Direct::solve(Amesos2Direct::matrix_t& A, Amesos2Direct::vector_t& x,
                     Amesos2Direct::vector_t& b ){


    using Teuchos::rcp;

    //std::cout<<"Trilinos Amesos2 version: "<<Amesos2::version()<<std::endl;

    std::string dstype = solver_type_to_string(dstype_);

    /*if(dstype == "SUPER_LU"){
        dstype = "SuperLU";

        if( !Amesos2::query(dstype.c_str()) ){
           throw std::logic_error("Amesos2 has not been configured to use SuperLU ");
        }

        std::cout<<"Using "<<dstype<<" direct solver"<<std::endl;
    }*/
    //else if(dstype == "KLU2"){
        dstype = "Klu";
        std::cout<<"Using "<<dstype<<" direct solver"<<std::endl;
    //}
    //else{
        //throw std::logic_error("Invalid direct solver type: " + dstype);
    //}



    typedef Amesos2Direct::matrix_t::trilinos_matrix_t mat_t;
    typedef Amesos2Direct::vector_t::trilinos_vector_t vec_t;

    auto* mat = A.get_matrix();

    if(!mat){
      throw std::logic_error("Epetra_CrsMatrix has not been created");
    }

    auto* rhs = b.raw_trilinos_vec_ptr();

    if(!rhs){
      throw std::logic_error("Epetra_MultiVector  rhs has not been created");
    }

    auto*& vec_rhs = rhs->operator()(0);

    for(uint_t t=0; t<vec_rhs->MyLength(); ++t){
        std::cout<<"Entry: "<<t<<" is: "<<vec_rhs->operator[](t)<<std::endl;
    }

    auto* sol = x.raw_trilinos_vec_ptr();

    if(!sol){
      throw std::logic_error("Epetra_MultiVector solution has not been created");
    }

    auto*& vec = sol->operator()(0);

    Epetra_LinearProblem Problem(mat, vec, vec_rhs);

    // Create solver interface to the requested solver with Amesos2 factory method
    /*std::unique_ptr<Amesos2::Solver<mat_t,vec_t>> solver(Amesos2::create<mat_t,vec_t>(dstype.c_str(),
                                                                                      mat, sol, rhs));*/
    Amesos factory;
    std::unique_ptr<Amesos_BaseSolver> solver(factory.Create(dstype.c_str(), Problem));

    if(solver.get() == nullptr){
        throw std::logic_error("Amesos2::Solver is not built");
    }

    Teuchos::ParameterList List;
    List.set("PrintTiming", true);
    List.set("PrintStatus", true);

    solver->SetParameters(List);
    //solver->SymbolicFactorization();
    //solver->NumericFactorization();
    solver->Solve();



    for(uint_t t=0; t<vec->MyLength(); ++t){
        std::cout<<"Entry: "<<t<<" is: "<<vec->operator[](t)<<std::endl;
    }

}

}
}
}

#endif
