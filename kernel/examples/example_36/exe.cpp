#include "kernel/base/config.h"
#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/common_uitls.h"
#include "kernel/maths/optimization/admm.h"
#include "kernel/maths/optimization/quadratic_problem.h"
#include "kernel/maths/direct_solvers/blaze_direct_solver.h"


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
        using kernel::maths::opt::ADMMData;
        using kernel::maths::solvers::BlazeDirectSolverConfig;
        using kernel::maths::solvers::BlazeDirectSolver;
        using kernel::maths::opt::QuadraticProblem;

        BlazeDirectSolverConfig config;
        BlazeDirectSolver solver(config);

        uint_t itrs = 1;
        ADMMData<DynMat<real_t>, DynVec<real_t>>  admm_data(solver, 0.5, 0.5, itrs, 0.5);
        ADMM<DynMat<real_t>, DynVec<real_t>> admm(admm_data);

        QuadraticProblem<DynMat<real_t>, DynVec<real_t>> qp;

        qp.P.resize(2, 2);
        qp.P(0, 0) = std::exp(-1.0);
        qp.P(0, 1) = 0.0;
        qp.P(1, 0) = 0.0;
        qp.P(1, 1) = 2.0;

        qp.A.resize(1, 2);
        qp.A(0, 0) = 0.0;
        qp.A(0, 1) = 0.0;

        // initialize the state vector
        qp.x.resize(2);
        qp.x[0] = 0.0;
        qp.x[1] = 0.0;

        qp.q.resize(2);
        qp.q[0] = -std::exp(-1.0);
        qp.q[1] = -2.0;

        qp.z.resize(qp.A.rows());

        for(uint_t i=0; i<qp.z.size(); ++i){
            qp.z[i] = 0.0;
        }

        admm.solve(qp);

        //std::cout<<"QProblem solution "<<qp.x(0)<<std::endl;

        DynMat<real_t> mat(3,3, 0.0);
        mat(0,0) = 1.;
        mat(1,1) = 1.;
        mat(2,2) = 1.;

        DynVec<real_t> x(3, 0.0);
        DynVec<real_t> rhs(3, 1.0);
        solve(mat, x, rhs);
        std::cout<<x<<std::endl;


    }
    catch(std::logic_error& error){

        std::cerr<<error.what()<<std::endl;
    }
    catch(...){
        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}





