#ifndef SERIAL_BATCH_GRADIENT_DESCENT_H
#define	SERIAL_BATCH_GRADIENT_DESCENT_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/optimization/utils/gd_info.h"

#include <boost/noncopyable.hpp>
#include <chrono>
#include <iostream>
#include <vector>

namespace cengine
{
        

/// \brief Implementation of the gradient descent (GC) algorithm
/// for solving optimization problems.
template<typename ErrorFunction>
class Gd: private boost::noncopyable
{
    
public:

    /// \brief The type used to measure the error
    typedef ErrorFunction error_t;
    
    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_t;
    
    /// \brief Constructor
    Gd(const GDControl& input);
    
    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    template<typename MatType, typename VecType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat,const VecType& v, HypothesisFuncType& h);

    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    template<typename MatType, typename VecType, typename HypothesisFuncType, typename RegularizerFunc>
    GDInfo solve(const MatType& mat,const VecType& v,
                 HypothesisFuncType& h, const RegularizerFunc& regularizer);

    /// \brief Reset the control
    void reset_control(const GDControl& control);
      
private:
    
    /// \brief The control data the GD solver is using
    GDControl input_;

    /// \brief The error function to use
    error_t err_function_;

    /// \brief actually solve the problem
    template<typename MatType, typename VecType,
             typename HypothesisFuncType, typename RegularizerFunc>
    GDInfo do_solve_(const MatType& mat,const VecType& v,
                     HypothesisFuncType& h, const RegularizerFunc* regularizer);
    
};

template<typename ErrorFunction>
inline
Gd<ErrorFunction>::Gd(const GDControl& input)
    :
      input_(input),
      err_function_()
{}

template<typename ErrorFunction>
template<typename MatType, typename VecType, typename HypothesisFuncType>
GDInfo 
Gd<ErrorFunction>::solve(const MatType& data, const VecType& y, HypothesisFuncType& h){
    
    typedef typename ErrorFunction::regularizer_t regularizer_t;
    regularizer_t* regularizer = nullptr;
    return do_solve_(data, y, h, regularizer);
}

template<typename ErrorFunction>
template<typename MatType, typename VecType,
         typename HypothesisFuncType, typename RegularizerFunc>
GDInfo
Gd<ErrorFunction>::solve(const MatType& data, const VecType& y,
                         HypothesisFuncType& h, const RegularizerFunc& regularizer){

    return do_solve_(data, y, h, &regularizer);
}

template<typename ErrorFunction>
template<typename MatType, typename VecType,
         typename HypothesisFuncType, typename RegularizerFunc>
GDInfo
Gd<ErrorFunction>::do_solve_(const MatType& data,const VecType& y,
                             HypothesisFuncType& h, const RegularizerFunc* regularizer){


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

    real_t j_old = err_function_.value(data, y).get_resource();
    real_t j_current = 0.0;

    const size_t ncoeffs = h.n_coeffs();

    while(input_.continue_iterations()){

        // get the gradients with respect to the coefficients
        auto j_grads = err_function_.gradients(data, y);

        //update the coefficients
        auto coeffs = h.coeffs();

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }

        // reset again the coeffs
        h.set_coeffs(coeffs);

        //recalculate...
        j_current = err_function_.value(data, y).get_resource();

        real_t error = std::fabs(j_current - j_old);
        input_.update_residual(error);
        uint_t itr = input_.get_current_iteration();

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
    info.nprocs = 1;
    info.nthreads = 1;
    info.converged = state.converged;
    info.residual = state.residual;
    info.tolerance = state.tolerance;
    info.niterations = state.num_iterations;
    return info;
}

template<typename ErrorFunction>
void
Gd<ErrorFunction>::reset_control(const GDControl& control){
    input_.reset(control);
}
        
}

#endif	/* GRADIENT_DESCENT_H */

