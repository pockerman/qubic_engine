#ifndef SERIAL_BATCH_GRADIENT_DESCENT_H
#define	SERIAL_BATCH_GRADIENT_DESCENT_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/optimization/utils/gd_info.h"
//#include "kernel/base/iteration_control.h"

#include <chrono>
#include <iostream>
#include <vector>

namespace cengine
{
        

/**
 * @brief Implementation of the gradient descent (GC) algorithm
 * for solving optimization problems.
 */

class Gd
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
    Gd(const GDControl& input);
    
    /**
     * @brief Solves the optimization problem. Returns information
     * about the performance of the solver.
     */
    template<typename MatType, typename VecType, typename ErrorFuncType, typename HypothesisFuncType>
    GDInfo solve(const MatType& mat,const VecType& v, 
                 const ErrorFuncType& fun, HypothesisFuncType& h);
      
private:
    
    /**
     * @brief The data the GD solver is using
     */
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
        auto j_grads = error_fun.coeff_grads(data, y);

        //update the coefficients
        std::vector<real_t>& coeffs = h.coeffs();
        
        for(uint_t c=0; c<ncoeffs; ++c){
            coeffs[c] -= input_.learning_rate*j_grads[c];
        }
        
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
        
        //zero the gradients
        for(uint_t c=0; c<j_grads.size(); ++c){
            j_grads[c] = 0.0;
        }
           
    }//itrs
    

    auto state = input_.get_state();
    
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    return info;  
}
        
}

#endif	/* GRADIENT_DESCENT_H */

