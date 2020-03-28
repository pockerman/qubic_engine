#ifndef NEWTON_SOLVER_H
#define NEWTON_SOLVER_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/utilities/iterative_algorithm_result.h"

#include <tuple>
#include <cmath>
#include <stdexcept>
#include <iostream>

namespace kernel {
namespace maths {
namespace solvers {

/// \brief Simple implementation of the Newton method
/// for solving non linear equations
template<typename PointTp, typename FunctionTp>
class NewtonSolver: public IterativeAlgorithmController
{
public:

    typedef PointTp point_t;
    typedef FunctionTp function_t;
    typedef IterativeAlgorithmResult output_t;

    /// \brief Constructor
    NewtonSolver(uint_t max_iterations, real_t exit_tolerance );

    /// \brief Solve for the given function by providing
    /// and initial point
    const std::tuple<output_t, point_t> solve(const point_t& x0,
                                              const function_t& function);
};

template<typename PointTp, typename FunctionTp>
NewtonSolver<PointTp, FunctionTp>::NewtonSolver(uint_t max_iterations, real_t exit_tolerance )
    :
    IterativeAlgorithmController(max_iterations, exit_tolerance)
{}

template<typename PointTp, typename FunctionTp>
const std::tuple<typename NewtonSolver<PointTp, FunctionTp>::output_t,
typename NewtonSolver<PointTp, FunctionTp>::point_t>
NewtonSolver<PointTp, FunctionTp>::solve(const NewtonSolver<PointTp, FunctionTp>::point_t& x0,
                                         const NewtonSolver<PointTp, FunctionTp>::function_t& function){

    typedef typename NewtonSolver<PointTp, FunctionTp>::output_t output_t;

    auto f = function(x0);

    if(std::fabs(f) < this->get_exit_tolerance()){
        this->update_residual(std::fabs(f));
        auto state = this->get_state();
        return std::make_tuple(state, x0);
    }

    auto fprime = function.gradient_mag(x0);

    /// The method cannot proceed if the
    /// the derivative of f is zero
    if(std::fabs(fprime) < kernel::KernelConsts::tolerance()){
        throw std::logic_error("Function derivative is zero. Cannot proceed");
    }

    auto xold = x0;
    while(this->continue_iterations()){

        if(this->show_iterations()){
            std::cout<<"Iteration state: "<<this->get_state()<<std::endl;
        }

        auto xn = xold - NewtonSolver<PointTp, FunctionTp>::point_t(function(xold)/function.gradient_mag(xold));
        this->update_residual(std::fabs(function(xn)));
        xold = xn;
    }

    auto state = this->get_state();
    return std::make_tuple(state, xold);
 }

}

}

}

#endif // NEWTON_SOLVER_H
