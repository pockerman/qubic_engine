#ifndef BISECTION_SOLVER_H
#define BISECTION_SOLVER_H

#include "kernel/base/types.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/utilities/iterative_algorithm_result.h"

#include <tuple>

namespace kernel {
namespace maths{
namespace solvers {

template<typename PointTp, typename FunctionTp>
class BisectionSolver: public IterativeAlgorithmController
{
public:

    typedef PointTp point_t;
    typedef FunctionTp function_t;
    typedef IterativeAlgorithmResult output_t;

    /// \brief Constructor
    BisectionSolver(uint_t max_iterations, real_t exit_tolerance );

    /// \brief Solve for the given function
    const std::tuple<output_t, point_t> solve(const point_t& p1,
                                              const point_t& p2,
                                              const function_t& function);
};

template<typename PointTp, typename FunctionTp>
BisectionSolver<PointTp, FunctionTp>::BisectionSolver(uint_t max_iterations, real_t exit_tolerance )
    :
    IterativeAlgorithmController(max_iterations, exit_tolerance)
{}

template<typename PointTp, typename FunctionTp>
const std::tuple<typename BisectionSolver<PointTp, FunctionTp>::output_t,
typename BisectionSolver<PointTp, FunctionTp>::point_t>
BisectionSolver<PointTp, FunctionTp>::solve(const BisectionSolver<PointTp, FunctionTp>::point_t& p1,
                                            const BisectionSolver<PointTp, FunctionTp>::point_t& p2,
                                            const BisectionSolver<PointTp, FunctionTp>::function_t& function){

    typedef typename BisectionSolver<PointTp, FunctionTp>::output_t output_t;

    auto f1 = function(p1);
    auto f2 = function(p2);
    /// quick return
    if( f1 * f2 > 0){
        return std::make_tuple(output_t(), point_t());
    }

    BisectionSolver<PointTp, FunctionTp>::point_t p1_copy = p1;
    BisectionSolver<PointTp, FunctionTp>::point_t p2_copy = p2;


    auto x = 0.5*(p1_copy + p2_copy);

    while(this->continue_iterations()){

        if(function(x) * function(p1) < 0){
            p2_copy = x;
        }
        else{
            p1_copy = x;
        }

        auto res = p1_copy.distance(p2_copy);
        this->update_residual(res);
        x = 0.5*(p1_copy + p2_copy);
    }

    auto state = this->get_state();
    return std::make_tuple(state, x);
 }

}
}

}

#endif // BISECTION_METHOD_H
