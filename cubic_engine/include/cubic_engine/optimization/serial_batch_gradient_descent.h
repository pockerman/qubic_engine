/* 
 * File:   gradient_descent.h
 * Author: david
 *
 * Created on June 20, 2016, 1:20 PM
 */

#ifndef SERIAL_BATCH_GRADIENT_DESCENT_H
#define	SERIAL_BATCH_GRADIENT_DESCENT_H

#include "parml/base/parml_config.h"
#include "parml/base/parml.h"
#include "parml/algorithms/gradient_descent_info_control.h"
#include "parml/algorithms/gradient_descent_type.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif 

#include <chrono>
#include <iostream>
#include <vector>

namespace parml
{
        
/**
 * @brief Implementation of the gradient descent (GC) algorithm
 * for solving optimization problems. There are two implementations
 * of the algorithm. Batch gradient descent and stochastic gradient descent.
 * This is the serial batched version of the algorithm
 */
template<>
class Gd<gradient_descent_type::Type::SERIAL_BGD> 
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
    Gd(const GDControl& data);
    
    /**
     * @brief Solves the optimization problem. Returns information
     * about the performance of the solver.
     */
    template<typename MatType,typename VecType,typename ErrorFuncType,typename HypothesisFuncType>
    GDInfo solve(const MatType& mat,const VecType& v, 
                 const ErrorFuncType& fun,HypothesisFuncType& h)const;
    
    
    template<typename MatType,typename VecType,typename ErrorFuncType,typename HypothesisFuncType>
    GDInfo solve(const MatType* mat,const VecType* v, 
                 const ErrorFuncType& fun,HypothesisFuncType& h)const{
        
        return solve(*mat,*v,fun,h);
    }
    
    
private:
    
    /**
     * @brief The data the GD solver is using
     */
    const GDControl data_;
    
};


template<typename MatType,typename VecType,typename ErrorFuncType,typename HypothesisFuncType>
GDInfo 
Gd<gradient_descent_type::Type::SERIAL_BGD>::solve(const MatType& mat,const VecType& v, 
                           const ErrorFuncType& error_fun,HypothesisFuncType& h)const{
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    //the info object to return
    GDInfo info;
    info.learning_rate = data_.learning_rate;
    info.type = gradient_descent_type::Type::SERIAL_BGD;
    
    //this should be scaled
    real_type Jold = error_fun.value(h); 
    real_type Jcur = utilities::scalar_value_traits<real_type>::zero();
    
    const size_type ncoeffs = h.n_coeffs();
    
    //the gradients of the error function.
    std::vector<real_type> Jgrads(ncoeffs,utilities::scalar_value_traits<real_type>::zero());
    
    size_type niterations = data_.max_n_iterations; 
    
    //start the iterations
    for(size_type itr=1; itr<=niterations; ++itr){
       
        //calculate the gradients
        error_fun.gradients(Jgrads,h);
        
        //update the coefficients
        std::vector<real_type>& coeffs = h.coeffs();
        
        for(size_type c=0; c<ncoeffs; ++c){
            coeffs[c] -= data_.learning_rate*Jgrads[c];
        }
        
        //recalculate...this is already scaled
        Jcur = error_fun.value(h);
        real_type error = std::fabs(Jcur-Jold);
        
        if(data_.show_iterations){
            
            std::cout<<"BatchGD: iteration: "<<itr<<std::endl;
            std::cout<<"\tJold: "<<Jold<<" Jcur: "<<Jcur
                     <<" error std::fabs(Jcur-Jold): "<<error
                     <<" exit tolerance: "<<data_.tolerance<<std::endl;
        }
        
        if(error < data_.tolerance){
            
            info.niterations = itr;
            info.residual = error;
            info.converged = true;
            break;
        }
        
        Jold = Jcur;
        
        //zero the gradients
        for(size_type c=0; c<Jgrads.size(); ++c){
            
            Jgrads[c] = utilities::scalar_value_traits<real_type>::zero();
        }
           
    }//itrs
    
    if(!info.converged)
        info.niterations = data_.max_n_iterations;
    
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    return info;  
}
        
}

#endif	/* GRADIENT_DESCENT_H */

