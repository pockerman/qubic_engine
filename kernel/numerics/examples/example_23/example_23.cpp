#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/numerics/optimization/admm.h"
#include "kernel/numerics/optimization/quadratic_problem.h"
#include "kernel/numerics/direct_solvers/blaze_direct_solver.h"


#include <cmath>
#include <iostream>
#include <string>

int main(){

    try{

        using kernel::real_t;
        using kernel::uint_t;
        using kernel::DynMat;
        using kernel::DynVec;

        using kernel::maths::opt::ADMM;
        using kernel::maths::opt::ADMMConfig;
        using kernel::maths::solvers::BlazeDirectSolverConfig;
        using kernel::maths::solvers::BlazeDirectSolver;
        using kernel::maths::opt::QuadraticProblem;

        BlazeDirectSolverConfig config;
        BlazeDirectSolver solver(config);

        uint_t itrs = 60;
        ADMMConfig<DynMat<real_t>, DynVec<real_t>>  admm_data(solver, .1, 0.01, itrs, 1.8);
        ADMM<DynMat<real_t>, DynVec<real_t>> admm(admm_data);

        QuadraticProblem<DynMat<real_t>, DynVec<real_t>> qp;

        qp.P.resize(2, 2);
        qp.P(0, 0) = std::exp(-1.0);
        qp.P(0, 1) = 0.0;
        qp.P(1, 0) = 0.0;
        qp.P(1, 1) = 2.0;

        qp.A.resize(1, 2);
        qp.A(0, 0) = 1.0;
        qp.A(0, 1) = 1.0;

        // initialize the state vector
        qp.x.resize(2);
        qp.x[0] = 1.0;
        qp.x[1] = 1.0;

        qp.q.resize(2);
        qp.q[0] = -std::exp(-1.0);
        qp.q[1] = -2.0;

        qp.z.resize(qp.A.rows(), 0.0);

        for(uint_t i=0; i<qp.z.size(); ++i){
            qp.z[i] = 0.0;
        }

        qp.u.resize(qp.A.rows());
        qp.u[0] = 1.0;

        admm.solve(qp);

        std::cout<<"QProblem solution: "<<std::endl;
        std::cout<<qp.x<<std::endl;
    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





