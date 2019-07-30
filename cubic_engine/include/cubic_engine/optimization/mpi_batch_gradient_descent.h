/* 
 * File:   mpi_par_batch_gradient_descent.h
 * Author: david
 *
 * Created on October 3, 2016, 10:00 PM
 */

#ifndef MPI_BATCH_GRADIENT_DESCENT_H
#define	MPI_BATCH_GRADIENT_DESCENT_H

#include "parml/base/parml_config.h"

#ifdef PARML_HAS_MPI

#include "parml/base/parml.h"
#include "parml/algorithms/gradient_descent_info_control.h"
#include "parml/algorithms/parallel_batch_gradient_descent_detail.h"
#include "parml/algorithms/gradient_descent_type.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif

#include "parframepp/runtime/mpi_rt_base.h"
#include "parframepp/base/parallel_indexed_object.h"

namespace parml
{


/**
 * @brief Implementation of the gradient descent (GD) algorithm
 * for solving optimization problems. There are two implementations
 * of the algorithm. Batch gradient descent and stochastic gradient descent.
 * This is the MPI batched version of the algorithm
 */

template<>
class Gd<gradient_descent_type::Type::MPI_BGD>:public parframepp::par_indexed_object
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
       
    const GDControl data_;
          
};

template<typename MatType,typename VecType,typename ErrorFuncType,typename HypothesisFuncType>
GDInfo 
Gd<gradient_descent_type::Type::MPI_BGD/*mpi_par,batch_gd*/>::solve(const MatType& mat,const VecType& v, 
                             const ErrorFuncType& error_fun,HypothesisFuncType& h)const{
    
    
    //the info object to return
    GDInfo info;
    info.nthreads = ParML::n_threads();
    info.nprocs   = ParML::n_processors();
    info.learning_rate = data_.learning_rate;
    info.type = gradient_descent_type::Type::MPI_BGD;
    
   //if we don't have partitions then signal an error
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    std::string msg = "Partitions for parallel object are empty";
    Assert(this->has_partitions()==true,ExeLogicError(msg));
    msg = "Data distribution types was not specified";
    Assert((data_.data_dist_type_flag == parframepp::data_dist_type::Type::COPY_ON_PROC) ||
           (data_.data_dist_type_flag == parframepp::data_dist_type::Type::CHUNK_ON_PROC),ExeLogicError(msg));
#endif
    
       std::chrono::time_point<std::chrono::system_clock> start, end;
       start = std::chrono::system_clock::now();
       
       //the object that holds the unscaled global error for the processor
       par_batch_gd_detail::SumOp<ErrorFuncType,HypothesisFuncType> sum_error(error_fun,h);
       
       //the object that holds the unscaled global gradients for the processor
       par_batch_gd_detail::GradOp<ErrorFuncType,HypothesisFuncType> grad_error(error_fun,h);
       
       size_type niterations = data_.max_n_iterations; 
       size_type pid = parframepp::rt_base<mpi_par>::pid();
       size_type master_pid = parframepp::rt_base<mpi_par>::master_process_id();
    
       //compute the processor local result
       //every process computes its local error 
       
       if(data_.data_dist_type_flag == parframepp::data_dist_type::Type::COPY_ON_PROC){
        //the data live also on the procs also so simply use the 
        //the partition chunk
            sum_error(this->partitions_[pid]);
       }
       else if(data_.data_dist_type_flag == parframepp::data_dist_type::Type::CHUNK_ON_PROC){
           //the data is on the master processor and
           //this processor simply has a chunk
           sum_error(this->get_local_indices(pid));   
       }
       
       real_type Jold_proc  = sum_error.get_result(); 
       real_type Jold       = utilities::scalar_value_traits<real_type>::zero();
       real_type Jcur       = utilities::scalar_value_traits<real_type>::zero();
       
       //reduce the result to the master process
       parframepp::rt_base<mpi_par>::reduce(Jold_proc,Jold,
                                           master_pid,std::plus<real_type>());
       
        //scale the global result
       if(pid == master_pid)
            Jold *= error_fun.scale_factor();
       
       
       std::vector<real_type> reduced_grads(h.n_coeffs(),utilities::scalar_value_traits<real_type>::zero());
       
        //start the iterations
        for(size_type itr=1; itr<=niterations; ++itr){
       
            if(data_.data_dist_type_flag == parframepp::data_dist_type::Type::COPY_ON_PROC){
                grad_error(this->partitions_[pid]);
            }
            else if(data_.data_dist_type_flag == parframepp::data_dist_type::Type::CHUNK_ON_PROC){
                grad_error(this->get_local_indices(pid)); 
            }
            
            //get the local gradients
            const std::vector<real_type>& grads = grad_error.get_result();
            size_type grads_size = grads.size();
            
            parframepp::rt_base<mpi_par>::reduce(&grads[0],grads_size,
                                                 &reduced_grads[0],master_pid,
                                                 std::plus<real_type>());
            
            //update the coefficients
            std::vector<real_type>& coeffs = h.coeffs();
            
            if(pid == master_pid){
                
               for(size_type c=0; c<coeffs.size(); ++c){
                    coeffs[c] -= data_.learning_rate*reduced_grads[c]*error_fun.grads_scale_factor();    
               }    
            }
            
            //broadcast the newly calculated coeffs
            parframepp::rt_base<mpi_par>::broadcast(&coeffs[0],coeffs.size(),master_pid);
            
            if(data_.data_dist_type_flag == parframepp::data_dist_type::Type::COPY_ON_PROC){
                //the data live also on the procs also so simply use the 
                //the partition chunk
                sum_error(this->partitions_[pid]);
            }
            else if(data_.data_dist_type_flag == parframepp::data_dist_type::Type::CHUNK_ON_PROC){
                //the data is on the master processor and
                //this processor simply has a chunk
                sum_error(this->get_local_indices(pid));   
            }
            
            real_type Jcur_proc = sum_error.get_result();
            
            //reduce the result to the master process
            parframepp::rt_base<mpi_par>::reduce(Jcur_proc,Jcur,
                                                 master_pid,
                                                 std::plus<real_type>());
            
            //dummy variable indicating whether the algorithm converged
            int converged = 0;
            
            if(pid == master_pid){
         
                //scale
                Jcur *= error_fun.scale_factor();
            
                real_type error = std::fabs(Jcur-Jold);
        
                if(data_.show_iterations){

                    parframepp::rt_root::Info<<"BatchGD: iteration: "<<itr<<std::endl;
                    parframepp::rt_root::Info<<"\tJold: "<<Jold<<" Jcur: "<<Jcur
                                            <<" error std::fabs(Jcur-Jold): "<<error
                                            <<" exit tolerance: "<<data_.tolerance<<std::endl;
                }
                
                if(error < data_.tolerance){

                    info.niterations = itr;
                    info.residual = error;
                    info.converged = true;
                    converged = 1;
                }
            }
            
            //broadcast whether the algorithm converged
            parframepp::rt_base<mpi_par>::broadcast(&converged,1,master_pid);
            
            if(converged == 1)
                break;
        
            Jold = Jcur; 
            Jcur = utilities::scalar_value_traits<real_type>::zero();
            
            for(size_type c=0; c<reduced_grads.size(); ++c)
                reduced_grads[c] = utilities::scalar_value_traits<real_type>::zero();

    }//itrs
        
    //if we reach here we did not converge
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    return info;     
}
       
}

#endif
#endif	/* MPI_PAR_BATCH_GRADIENT_DESCENT_H */

