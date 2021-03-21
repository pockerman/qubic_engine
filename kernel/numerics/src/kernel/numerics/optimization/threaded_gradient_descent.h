#ifndef THREADED_BATCH_GRADIENT_DESCENT_H
#define THREADED_BATCH_GRADIENT_DESCENT_H

#include "kernel/base/types.h"
#include "kernel/numerics/optimization/utils/gd_control.h"
#include "kernel/numerics/optimization/utils/gd_info.h"
#include "kernel/numerics/functions/dummy_function.h"

#include <boost/noncopyable.hpp>
#include <chrono>
#include <iostream>
#include <vector>

namespace kernel{
namespace maths {
namespace opt {

///
/// \brief Implementation of the gradient descent (GC) algorithm
/// for solving optimization problems.
///
template<typename ErrorFunction>
class ThreadedGd: private boost::noncopyable
{

public:

    /// \brief The type used to measure the error
    typedef ErrorFunction error_t;

    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_t;

    /// \brief Constructor
    ThreadedGd(const GDConfig& input);

    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    template<typename MatType, typename VecType, typename HypothesisFuncType,
             typename Executor, typename Options>
    GDInfo solve(const MatType& mat,const VecType& v, HypothesisFuncType& h,
                 Executor& executor, const Options& options);

    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    template<typename MatType, typename VecType, typename HypothesisFuncType,
             typename RegularizerFuncType, typename Executor, typename Options>
    GDInfo solve(const MatType& mat,const VecType& v, HypothesisFuncType& h,
                 const RegularizerFuncType& regularizer, Executor& executor,
                 const Options& options);

    /// \brief Reset the control
    void reset_control(const GDConfig& control);

private:

    /// \brief The data the GD solver is using
    GDConfig input_;

    /// \brief The error function to use
    error_t err_function_;

    /// \brief actually solve the optimization problem
    template<typename MatType, typename VecType, typename HypothesisFuncType,
             typename RegularizerFuncType, typename Executor, typename Options>
    GDInfo do_solve_(const MatType& mat,const VecType& v, HypothesisFuncType& h,
                     const RegularizerFuncType* regularizer, Executor& executor,
                     const Options& options);
};
template<typename ErrorFunction>
inline
ThreadedGd<ErrorFunction>::ThreadedGd(const GDConfig& input)
    :
      input_(input),
      err_function_()
{}

template<typename ErrorFunction>
template<typename MatType, typename VecType, typename HypothesisFuncType,
         typename Executor, typename Options>
GDInfo
ThreadedGd<ErrorFunction>::solve(const MatType& mat,const VecType& v, HypothesisFuncType& h,
                                 Executor& executor, const Options& options){

    typedef typename ErrorFunction::regularizer_t regularizer_t;
    regularizer_t* regularizer = nullptr;
    return do_solve_(mat, v, h, regularizer, executor, options);
}

template<typename ErrorFunction>
template<typename MatType, typename VecType, typename HypothesisFuncType,
         typename RegularizerFuncType, typename Executor, typename Options>
GDInfo
ThreadedGd<ErrorFunction>::solve(const MatType& mat,const VecType& v, HypothesisFuncType& h,
                                 const RegularizerFuncType& regularizer, Executor& executor,
                                 const Options& options){

    return do_solve_(mat, v, h, regularizer, executor, options );
}

template<typename ErrorFunction>
template<typename MatType, typename VecType, typename HypothesisFuncType,
         typename RegularizerFuncType, typename Executor, typename Options>
GDInfo
ThreadedGd<ErrorFunction>::do_solve_(const MatType& mat,const VecType& v, HypothesisFuncType& h,
                                     const RegularizerFuncType* regularizer, Executor& executor,
                                     const Options& options){


    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // set the hypothesis function of the
    // error function
    err_function_.set_hypothesis_function(h);

    if(regularizer != nullptr){
        err_function_.set_regularizer_function(*regularizer);
    }

    //the info object to return
    GDInfo info;
    info.learning_rate = input_.learning_rate;

    // this should block
    auto result = err_function_.value(mat, v, executor, options);

    real_t j_old = *result.get_or_wait().first;
    real_t j_current = 0.0;

    const uint_t ncoeffs = h.n_coeffs();

    while(input_.continue_iterations()){

        // get the gradients with respect to the coefficients
        auto j_grads_result = err_function_.gradients(mat, v, executor, options);
        auto j_grads = *(j_grads_result.get().first);

        //update the coefficients
        auto coeffs = h.coeffs();

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }

        // reset again the coeffs
        h.set_coeffs(coeffs);

        //recalculate...
        result = err_function_.value(mat, v, executor, options);

        j_current = *result.get_or_wait().first;

        real_t error = std::fabs(j_current - j_old);

        input_.update_residual(error);
        uint_t itr = input_.get_current_iteration();

        if(input_.show_iterations()){

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
    info.nprocs = 1;
    info.nthreads = executor.get_n_threads();
    info.converged = state.converged;
    info.residual = state.residual;
    info.tolerance = state.tolerance;
    info.niterations = state.num_iterations;
    return info;
}



template<typename ErrorFunction>
void
ThreadedGd<ErrorFunction>::reset_control(const GDConfig& control){
    input_.reset(control);
}
}
}
}

#endif // THREADED_BATCH_GRADIENT_DESCENT_H
