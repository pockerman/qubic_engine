#ifndef SERIAL_BATCH_GRADIENT_DESCENT_H
#define	SERIAL_BATCH_GRADIENT_DESCENT_H

#include "kernel/base/types.h"
#include "kernel/maths/optimization/utils/gd_control.h"
#include "kernel/maths/optimization/utils/gd_info.h"

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
class Gd: private boost::noncopyable
{
    
public:

    ///
    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    ///
    typedef GDInfo output_t;
    
    ///
    /// \brief Constructor
    ///
    Gd(const GDConfig& input);
    
    ///
    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    ///
    template<typename MatType, typename VecType, typename FunctionTp>
    GDInfo solve(const MatType& mat,const VecType& v, FunctionTp& h);

    ///
    /// \brief Solve for the coefficients that minimize
    /// the given function
    ///
    template<typename FunctionTp>
    GDInfo solve(FunctionTp& function);

    ///
    /// \brief Reset the control
    ///
    void reset_configuration(const GDConfig& control);
      
private:
    
    ///
    /// \brief The control data the GD solver is using
    ///
    GDConfig input_;

    ///
    /// \brief The error function to use
    ///
    error_t err_function_;

    ///
    /// \brief actually solve the problem
    ///
    template<typename MatType, typename VecType, typename FunctionTp>
    GDInfo do_solve_(const MatType& mat,const VecType& v, FunctionTp& h);
    
};


inline
Gd::Gd(const GDConfig& input)
    :
      input_(input),
      err_function_()
{}


template<typename MatType, typename VecType, typename FunctionTp>
GDInfo 
Gd::solve(const MatType& data, const VecType& y, FunctionTp& function){
    return do_solve_(data, y, function);
}


template<typename FunctionTp>
GDInfo
Gd::solve(FunctionTp& function){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //the info object to return
    GDInfo info;
    info.learning_rate = input_.learning_rate;

    auto coeffs = function.coeffs();
    const uint_t ncoeffs = function.n_coeffs();
    auto val_old = function.value(coeffs);

    while(input_.continue_iterations()){

        auto grads = function.gradients(coeffs);

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*grads[c];
        }

        // reset again the coeffs
        function.set_coeffs(coeffs);

        auto new_val = function.value(function.coeffs());

        real_t error = std::fabs(new_val - val_old);
        input_.update_residual(error);
        uint_t itr = input_.get_current_iteration();

        if(input_.show_iterations()){

            std::cout<<">GD: iteration: "<<itr<<std::endl;
            std::cout<<"\t eta: "<<input_.learning_rate
                     <<" ABS error: "<<error
                     <<" Exit Tol: "<<input_.get_exit_tolerance()<<std::endl;
        }
        val_old = new_val;
    }

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

template<typename MatType, typename VecType, typename FunctionTp>
GDInfo
Gd::do_solve_(const MatType& data,const VecType& y, FunctionTp& function){


    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();


    //the info object to return
    GDInfo info;
    info.learning_rate = input_.learning_rate;

    real_t j_old = function.value(data, y).get_resource();
    real_t j_current = 0.0;

    const uint_t ncoeffs = function.n_coeffs();

    while(input_.continue_iterations()){

        // get the gradients with respect to the coefficients
        auto j_grads = function.gradients(data, y);

        //update the coefficients
        auto coeffs = function.coeffs();

        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }

        // reset again the coeffs
        function.update_model(coeffs);

        //recalculate...
        j_current = function.value(data, y).get_resource();

        real_t error = std::fabs(j_current - j_old);
        input_.update_residual(error);
        uint_t itr = input_.get_current_iteration();

        if(input_.show_iterations()){

            std::cout<<"GD: iteration: "<<itr<<std::endl;
            std::cout<<"\t eta: "<<input_.learning_rate
                     <<" absolute error: "<<error
                     <<" exit tolerance: "<<input_.get_exit_tolerance()<<std::endl;
        }

        j_old = j_current;
    }

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

inline
void
Gd::reset_configuration(const GDConfig& control){
    input_.reset(control);
}
        
}
}
}

#endif	/* GRADIENT_DESCENT_H */

