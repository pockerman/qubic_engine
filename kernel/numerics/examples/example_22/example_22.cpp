#include "kernel/base/config.h"

#ifdef USE_TRILINOS

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/numerics/direct_solvers/amesos_solver.h"
#include "kernel/numerics/direct_solvers/direct_solver_type.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#include "kernel/maths/trilinos_epetra_multivector.h"

#include <cmath>
#include <iostream>

int main(){

    try{

            using kernel::real_t;
            using kernel::uint_t;
            using kernel::maths::solvers::AmesosDirect;
            using kernel::numerics::TrilinosEpetraMatrix;
            using kernel::numerics::TrilinosEpetraVector;
            using kernel::maths::algebra::TrilinosEpetraMultiVector;


            typedef TrilinosEpetraMatrix::row_entries_t row_entries_t;
            TrilinosEpetraMatrix A;

            // construct a 6x6 matrix
            A.init(6, 6, 6);

            row_entries_t entries = {7,  6,  3, 4,  5, 8}; //{7,  6.,  -3,  1,  -1, 2.};
            A.set_row_entries(0, entries);

            //for(uint_t i=0; i<A.m(); ++i)
            //    A.set_entry(i,i,1.);

            entries = {0,  1.,  0,  0,  7.,  0};
            //A.set_row_entries(1, entries);

            entries = {0,  0,  1.,  0,  0,  0};
            //A.set_row_entries(2, entries);

            entries = {0, 0,  0,  1.,  0,  0};
            //A.set_row_entries(3, entries);

            entries = {0,  0,  0,  0,  1.,  0};
            //A.set_row_entries(4, entries);

            entries = { 1.,  2.,  3.,  4., 5.,  6. };
            //A.set_row_entries(5, entries);
            A.fill_completed();

            A.print(std::cout);

            for(uint_t i=0; i<A.m(); ++i){
                for(uint_t j=0; j<A.m(); ++j){
                    std::cout<<"Entry: "<<i<<","<<j<<","<<A.entry(i,j)<<std::endl;
                }
            }

            /*TrilinosEpetraMultiVector x(1, A.m(), 0.0);
            TrilinosEpetraMultiVector b(1, A.m(), 0.0);
            real_t vals[6] = {-7,18,3,17,18,28};

            for(uint_t i=0; i<b.size(); ++i){
                b.set_entry(0, i, vals[i]);
            }

            std::cout<<"Solving system"<<std::endl;
            // create the solver
            Amesos2Direct solver(kernel::maths::solvers::DirectSolverType::KLU2);
            solver.solve(A, x, b);*/

            /*std::cout<<"Printing solution..."<<std::endl;
            for(uint_t i=0; i<x.size(); ++i){
               std::cout<<"item: "<<i<<", "<<x.get(0,i)<<std::endl;
            }*/


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





