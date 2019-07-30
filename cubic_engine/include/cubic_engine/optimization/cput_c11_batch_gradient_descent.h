/* 
 * File:   cput_c11_batch_gradient_descent.h
 * Author: david
 *
 * Created on October 3, 2016, 9:51 PM
 */

#ifndef CPUT_C11_BATCH_GRADIENT_DESCENT_H
#define	CPUT_C11_BATCH_GRADIENT_DESCENT_H


#include "parml/base/parml.h"
#include "parml/base/parml_config.h"
#include "parml/algorithms/gradient_descent_info_control.h"
#include "parml/algorithms/parallel_batch_gradient_descent_detail.h"
#include "parml/algorithms/gradient_descent_type.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif


#include "parframepp/parallel/task_base.h"
#include "parframepp/runtime/run_time.h"
#include "parframepp/models/parallel_reduce.h"
#include "parframepp/base/parallel_indexed_object.h"
#include "parframepp/models/reduction_state.h"


namespace parml
{
    
/**
 * @brief Implementation of the  batch gradient descent (GD) algorithm
 * for solving optimization problems. There are two implementations
 * of the algorithm. Batch gradient descent and stochastic gradient descent.
 * This is the multi-threaded batched version of the algorithm
 */

template<>
class Gd<gradient_descent_type::Type::CPUT_C11_BGD>: public parframepp::par_indexed_object
{
   
public:
    
    /**
     * @brief Expose the type that is returned by this object
     * when calling its solve functions.
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

template<typename MatType,
         typename VecType,
         typename ErrorFuncType,
         typename HypothesisFuncType>
GDInfo 
Gd<gradient_descent_type::Type::CPUT_C11_BGD>::solve(const MatType& mat,const VecType& v, 
                                                     const ErrorFuncType& error_fun,
                                                     HypothesisFuncType& h)const{
    
    
    //the info object to return
    GDInfo info;
    info.nthreads = ParML::n_threads();
    info.learning_rate = data_.learning_rate;
    info.type = gradient_descent_type::Type::CPUT_C11_BGD;
    
   //if we don't have partitions then signal an error
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = "Partitions for parallel object are empty";
    Assert(this->has_partitions()==true,ExeLogicError(msg));
#endif
    
       std::chrono::time_point<std::chrono::system_clock> start, end;
       start = std::chrono::system_clock::now();
       
       size_type niterations = data_.max_n_iterations; 
    
       //the object that holds the unscaled global error
       par_batch_gd_detail::SumOp<ErrorFuncType,HypothesisFuncType> sum_error(error_fun,h);
       
       //the object that holds the unscaled global gradients
       par_batch_gd_detail::GradOp<ErrorFuncType,HypothesisFuncType> grad_error(error_fun,h);
       
       parframepp::reduction_state sum_state;
       parframepp::reduction_state grad_state;
       
       //compute the error
       parframepp::parallel_reduce(this->partitions_,sum_error,sum_state);
       
       real_type Jold  = sum_error.get_result(); 
       
       //scale the global result
       Jold *= error_fun.scale_factor();
       
        //start the iterations
        for(size_type itr=1; itr<=niterations; ++itr){
       
            parframepp::parallel_reduce(this->partitions_,grad_error,grad_state);
            
            const std::vector<real_type>& grads = grad_error.get_result();
        
            //update the coefficients
            std::vector<real_type>& coeffs = h.coeffs();
        
            for(size_type c=0; c<coeffs.size(); ++c){
                coeffs[c] -= data_.learning_rate*grads[c]*error_fun.grads_scale_factor();    
            }
        
            parframepp::parallel_reduce(this->partitions_,sum_error,sum_state);
            
            real_type Jcur = sum_error.get_result();
         
            //scale
            Jcur *= error_fun.scale_factor();
            
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
        }//itrs
       
       if(!info.converged)
         info.niterations = data_.max_n_iterations;
        
    //if we reach here we did not converge
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    return info;  
    
}
    
}



#endif	/* CPUT_C11_BATCH_GRADIENT_DESCENT_H */

