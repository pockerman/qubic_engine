#ifndef THREADED_BATCH_GRADIENT_DESCENT_H
#define THREADED_BATCH_GRADIENT_DESCENT_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/optimization/utils/gd_info.h"

#include <chrono>
#include <iostream>
#include <vector>

namespace cengine
{

/**
 * @brief Implementation of the gradient descent (GC) algorithm
 * for solving optimization problems.
 */

class ThreadedGd
{

public:

    /**
     * @brief Expose the type that is returned by this object
     * when calling its solve functions
     */
    typedef GDInfo output_type;

    /**
     * @brief Constructor
     */
    ThreadedGd(const GDControl& input);

    /**
     * @brief Solves the optimization problem. Returns information
     * about the performance of the solver.
     */
    template<typename MatType, typename VecType,
             typename ErrorFuncType, typename HypothesisFuncType,
             typename Executor, typename Options>
    GDInfo solve(const MatType& mat,const VecType& v,
                 ErrorFuncType& error_fun, HypothesisFuncType& h,
                 Executor& executor, const Options& options);

private:

    /**
     * @brief The data the GD solver is using
     */
    GDControl input_;

};

inline
ThreadedGd::ThreadedGd(const GDControl& input)
    :
      input_(input)
{}

template<typename MatType, typename VecType,
         typename ErrorFuncType, typename HypothesisFuncType,
         typename Executor, typename Options>
GDInfo
ThreadedGd::solve(const MatType& mat,const VecType& v,
                  ErrorFuncType& error_fun, HypothesisFuncType& h,
                  Executor& executor, const Options& options){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //the info object to return
    GDInfo info;
    info.learning_rate = input_.learning_rate;

    // this should block
    auto result = error_fun.value(mat, v, executor, options);

    real_t j_old = *result.get_or_wait().first;
    real_t j_current = 0.0;

    const uint_t ncoeffs = h.n_coeffs();

    while(input_.continue_iterations()){

        // get the gradients with respect to the coefficients
        auto j_grads_result = error_fun.gradients(mat, v, executor, options);
        auto j_grads = *(j_grads_result.get().first);

        //update the coefficients
        auto coeffs = h.coeffs();

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }

        // reset again the coeffs
        h.set_coeffs(coeffs);

        //recalculate...
        result = error_fun.value(mat, v, executor, options);

        j_current = *result.get_or_wait().first;

        real_t error = std::fabs(j_current - j_old);

        //input_.update_residual(error);
        uint_t itr = input_.get_current_tteration();

        if(input_.show_iterations){

            std::cout<<"BatchGD: iteration: "<<itr<<std::endl;
            std::cout<<"\tJold: "<<j_old<<" Jcur: "<<j_current
                     <<" error std::fabs(Jcur-Jold): "<<error
                     <<" exit tolerance: "<<input_.get_exit_tolerance()<<std::endl;
        }

        j_old = j_current;

    }//itrs

    auto state = input_.get_state();
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    return info;
}
}

#endif // THREADED_BATCH_GRADIENT_DESCENT_H
