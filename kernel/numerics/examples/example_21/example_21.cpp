#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/numerics/direct_solvers/amesos_solver.h"
#include "kernel/numerics/direct_solvers/direct_solver_type.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_multivector.h"

#include <cmath>
#include <iostream>

int main(){

    try{

            using kernel::real_t;
            using kernel::uint_t;
            using kernel::maths::solvers::AmesosDirect;
            using  kernel::maths::solvers::AmesosDirectConfig;
            using kernel::numerics::TrilinosEpetraMatrix;
            using kernel::maths::algebra::TrilinosEpetraMultiVector;

            typedef TrilinosEpetraMatrix::row_entries_t row_entries_t;
            TrilinosEpetraMatrix A;

            // construct the matrix
            A.init(6, 6, 6);

            row_entries_t entries = {7,  0,  -3, 0,  -1, 0};
            A.set_row_entries(0, entries);

            entries = {2,  8,  0,  0,  0,  0};
            A.set_row_entries(1, entries);

            entries = {0,  0,  1.,  0,  0,  0};
            A.set_row_entries(2, entries);

            entries = {-3, 0,  0,  5,  0,  0};
            A.set_row_entries(3, entries);

            entries = {0,  -1, 0,  0,  4,  0};
            A.set_row_entries(4, entries);

            entries = { 0,  0,  0,  -2, 0,  6. };
            A.set_row_entries(5, entries);

            A.fill_completed();

            // solution and RHS
            TrilinosEpetraMultiVector x(1, A.m(), 0.0);
            TrilinosEpetraMultiVector b(1, A.m(), 0.0);
            real_t vals[6] = {-7,18,3,17,18,28};

            for(uint_t i=0; i<b.size(); ++i){
                b.set_entry(0, i, vals[i]);
            }

            std::cout<<"Solving system"<<std::endl;
            AmesosDirectConfig options;
            options.dstype = kernel::maths::solvers::DirectSolverType::LU;

            // create the solver
            AmesosDirect solver(options);
            solver.solve(A, x, b);

            // Print the solution
            //
            // Should be:
            //
            //  [[1]
            //   [2]
            //   [3]
            //   [4]
            //   [5]
            //   [6]]
            //
            std::cout<<"Printing solution..."<<std::endl;
            for(uint_t i=0; i<x.size(); ++i){
               std::cout<<"item: "<<i<<", "<<x.get(0,i)<<std::endl;
            }


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





