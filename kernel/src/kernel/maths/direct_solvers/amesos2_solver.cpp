#include "kernel/maths/direct_solvers/amesos2_solver.h"

#ifdef USE_TRILINOS

#include "kernel/maths/solvers/solver_package_type.h"

#include <Teuchos_RCP.hpp>
//#include <Teuchos_ScalarTraits.hpp>
//#include <Teuchos_oblackholestream.hpp>
//#include <Teuchos_Tuple.hpp>
//#include <Teuchos_VerboseObject.hpp>

#include <Amesos2.hpp>

#include <stdexcept>

namespace kernel {
namespace maths {
namespace solvers {

Amesos2Direct::Amesos2Direct(DirectSolverType dstype)
    :
      DirectSolverBase<kernel::numerics::TrilinosEpetraMatrix,
                       kernel::numerics::TrilinosEpetraVector>(SolverPackageType::TRI_AMESOS2),
      dstype_(dstype)
{}

void
Amesos2Direct::solve(const Amesos2Direct::matrix_t& A, Amesos2Direct::vector_t& x,
                     const Amesos2Direct::vector_t& b ){


    using Teuchos::rcp;
    std::string dstype = solver_type_to_string(dstype_);

    if(dstype == "SUPER_LU"){
        dstype = "SuperLU";

        if( !Amesos2::query(dstype.c_str()) ){
           throw std::logic_error("Amesos2 has not been configured to use SuperLU ");
        }

        std::cout<<"Using "<<dstype<<" direct solver"<<std::endl;
    }
    else if(dstype == "KLU2"){
        dstype = "Klu2";
        std::cout<<"Using "<<dstype<<" direct solver"<<std::endl;
    }
    else{
        throw std::logic_error("Invalid direct solver type: " + dstype);
    }



    typedef Amesos2Direct::matrix_t::trilinos_matrix_t mat_t;
    typedef Amesos2Direct::vector_t::trilinos_vector_t vec_t;

    const auto* mat = A.get_matrix();

    if(!mat){
      throw std::logic_error("Epetra_CrsMatrix has not been created");
    }

    const auto* rhs = b.get_vector();

    if(!rhs){
      throw std::logic_error("Epetra_Vector  rhs has not been created");
    }

    auto* sol = x.get_vector();

    if(!sol){
      throw std::logic_error("Epetra_Vector  solution has not been created");
    }


    // Create solver interface to Superlu with Amesos2 factory method
    //Amesos2::Solver<mat_t,vec_t>* solver = Amesos2::create<mat_t,vec_t>(dstype.c_str(), mat, sol, rhs);

    //solver->symbolicFactorization().numericFactorization().solve();

}

}
}
}

#endif
