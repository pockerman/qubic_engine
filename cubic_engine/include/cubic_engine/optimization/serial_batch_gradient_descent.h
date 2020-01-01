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
        

/**
 * @brief Implementation of the gradient descent (GC) algorithm
 * for solving optimization problems.
 */

class Gd: private boost::noncopyable
{
    
public:
    
    /// \brief Expose the type that is returned by this object
    /// when calling its solve functions
    typedef GDInfo output_type;
    
    /// \brief Constructor
    Gd(const GDControl& input);
    
    /// \brief Solves the optimization problem. Returns information
    /// about the performance of the solver.
    template<typename MatType, typename VecType, typename ErrorFuncType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat,const VecType& v, 
                 const ErrorFuncType& fun, HypothesisFuncType& h);

    /// \brief Reset the control
    void reset_control(const GDControl& control){input_ = control;}
      
private:
    
    /// \brief The control data the GD solver is using
    GDControl input_;
    
};

inline
Gd::Gd(const GDControl& input)
    :
      input_(input)
{}

template<typename MatType,typename VecType,typename ErrorFuncType,typename HypothesisFuncType>
GDInfo 
Gd::solve(const MatType& data,const VecType& y,
                        const ErrorFuncType& error_fun,HypothesisFuncType& h){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    //the info object to return
    GDInfo info;
    info.learning_rate = input_.learning_rate;

    real_t j_old = error_fun.value(data, y);
    real_t j_current = 0.0;
    
    const size_t ncoeffs = h.n_coeffs();
    
    while(input_.continue_iterations()){

        // get the gradients with respect to the coefficients
        auto j_grads = error_fun.gradients(data, y);

        //update the coefficients
        auto coeffs = h.coeffs();
        
        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }

        // reset again the coeffs
        h.set_coeffs(coeffs);
        
        //recalculate...
        j_current = error_fun.value(data, y);

        real_t error = std::fabs(j_current - j_old);
        input_.update_residual(error);
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
    info.nprocs = 1;
    info.nthreads = 1;
    info.converged = state.converged;
    info.residual = state.residual;
    info.tolerance = state.tolerance;
    info.niterations = state.num_iterations;
    return info;  
}
        
}

#endif	/* GRADIENT_DESCENT_H */

