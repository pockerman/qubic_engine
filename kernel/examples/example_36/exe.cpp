#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/maths/trilinos_epetra_multivector.h"
#include "kernel/maths/trilinos_epetra_matrix.h"
#include "kernel/maths/trilinos_epetra_vector.h"
#include "kernel/maths/optimization/admm.h"
#include "kernel/maths/optimization/quadratic_problem.h"
#include "kernel/maths/direct_solvers/amesos_solver.h"


#include <cmath>
#include <iostream>
#include <string>

int main(){

    try{

        using kernel::real_t;
        using kernel::uint_t;
        using kernel::numerics::TrilinosEpetraMatrix;
        using kernel::numerics::TrilinosEpetraVector;
        using kernel::maths::algebra::TrilinosEpetraMultiVector;
        using kernel::maths::opt::ADMM;
        using kernel::maths::opt::ADMMData;
        using kernel::maths::solvers::AmesosDirect;
        using kernel::maths::solvers::AmesosDirectOptions;
        using kernel::maths::solvers::DirectSolverBase;
        using kernel::maths::opt::QuadraticProblem;

        // number of constraints
        const uint_t m = 30;

        // number of state variables
        const uint_t n = 20;

        // create a random matrix

        AmesosDirectOptions amesos_dir_options;
        AmesosDirect direct_solver(amesos_dir_options);

        uint_t itrs = 100;
        ADMMData<TrilinosEpetraMatrix,TrilinosEpetraMultiVector>  admm_data(direct_solver, 0.5, 0.5, itrs, 0.5);
        ADMM<TrilinosEpetraMatrix,TrilinosEpetraMultiVector> admm(admm_data);

        QuadraticProblem<TrilinosEpetraMatrix,TrilinosEpetraMultiVector> qp;

        qp.P.init(2, 2, 0);
        qp.P.set_entry(0, 0, std::exp(-1.0));
        qp.P.set_entry(0, 1, 0.0);
        qp.P.set_entry(1, 0, 0.0);
        qp.P.set_entry(1, 1, 2.0);

        qp.A.init(1, 2, 0);
        qp.A.set_entry(0, 0, 1.0);
        qp.A.set_entry(0, 1, 1.0);

        // initialize the state vector
        qp.x.init(1, 2, 0.0);

        qp.q.init(1, 2, 0.0);
        qp.q.set_entry(0, 0, -std::exp(-1.0));
        qp.q.set_entry(0, 1, -2.0);

        qp.z.init(1, 1, 0.0);

        admm.solve(qp);

        std::cout<<"QProblem solution "<<qp.x(0)<<std::endl;

    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





