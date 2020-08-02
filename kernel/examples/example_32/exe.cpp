#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/maths/direct_solvers/amesos2_solver.h"
#include "kernel/maths/direct_solvers/direct_solver_type.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"

#include <cmath>
#include <iostream>

int main(){

    try{

            using kernel::real_t;
            using kernel::uint_t;
            using kernel::maths::solvers::Amesos2Direct;
            using kernel::numerics::TrilinosEpetraMatrix;
            using kernel::numerics::TrilinosEpetraVector;

            // create the solver
            Amesos2Direct solver(kernel::maths::solvers::DirectSolverType::KLU2);

            TrilinosEpetraMatrix A;
            TrilinosEpetraVector x;
            TrilinosEpetraVector b;

            solver.solve(A, x, b);



    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}
#else
#include <iostream>
int main(){

    std::cerr<<"This example requires Trilinos. You need to configure with Trilinos support"<<std::endl;
    return 0;
}
#endif





