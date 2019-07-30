/* 
 * File:   smo.h
 * Author: david
 *
 * Created on July 7, 2016, 2:49 PM
 */

#ifndef SMO_H
#define	SMO_H

#include "parml/base/parml.h"
#include "parml/base/parml_runtime.h"
#include "parml/base/data_adaptor.h"
#include "parml/base/data_point_type.h"
#include "parml/base/algorithm_control.h"
#include "parml/base/algorithm_info.h"
#include "parml/parallel_utils/parallel_indexed_object.h"
#include "parml/algorithms/smo_partial_sum_task.h"
#include "parml/base/parml_config.h"

#ifdef PARML_HAS_MPI
#include "parml/parallel_utils/data_distribution_type.h"
#endif 


#include "parframepp/parallel/task_base.h"
#include "parframepp/runtime/run_time.h"
#include "parframepp/models/parallel_reduce.h"

#include "utilities/inner_product.h"
#include "utilities/map_utilities.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif 

#include <vector>
#include <map>
#include <utility>
#include <chrono>
#include <random>
#include <ostream>

namespace parml
{
    
    namespace smo_detail
    {
        
        /**
        * @brief Clip the given value in the given range
        */
        void clip(real_type& a, real_type H, real_type L); 
        
        /**
         * @brief Get the list of indices that have non zero error in the given map
         */
        void get_non_zero_indices(const std::map<size_type,real_type>& error_cache,
                                  std::vector<size_type>& indices,real_type tol=ParML::tolerance());
        
        
        /**
         * @brief Determine H,L
         */
        void determine_H_and_L(real_type& H,real_type& L,
                               real_type C,real_type ai,real_type aj,int yi,int yj);
        
        
        /**
         * @brief Determine the beta coefficient
         */
        void determine_beta_coeff(real_type& beta,
                                  real_type C, real_type ai,real_type aj,
                                  real_type b1,real_type b2);    
    }//smo_detail
    
/**
 * Implementation of the SMO (sequential minimal optimization) algorithm
 * from Platt 1999 for training support vector machines
 */
template<typename RM> class Smo;


/**
 * @brief A small struct that holds data
 * about the SMO algorithm
 */    
struct SmoControl: public AlgControl{
    
    /**
     * @brief The learning rate
     */ 
    real_type C;
    
    /**
     * @brief Tolerance below which a value is considered as zero
     */
    real_type small_value;
    
    /**
     * @brief Number of features the data set has
     */
    size_type n_features;
    
    
    /**
     * @brief Number of training examples
     */
    size_type n_training_examples;
    
    /**
     * @brief Constructor.
     */
    SmoControl(size_type itrs,real_type C_)
            :
            AlgControl(itrs,ParML::tolerance()),
            C(C_),
            small_value(ParML::tolerance()),
            n_features(ParML::iuint()),
            n_training_examples(ParML::iuint())
    {}
    
    /**
     * @brief print the control data to the given stream
     */
    virtual std::ostream& print(std::ostream& out)const;
};

inline
std::ostream& 
operator<<(std::ostream& out, const SmoControl& c){
    return c.print(out);
}


/**
 * @brief The object to return when the gradient descent algorithm finishes
 */
struct SmoInfo: public AlgInfo{
    
    /**
     * @brief Constructor
     */
    SmoInfo()
            :
            AlgInfo()
    {}        
};

inline
std::ostream&
operator<<(std::ostream& out,const SmoInfo& info){
    return info.print(out);
}


class SmoBase
{
    
    
public:
    
    /**
      * @brief The output returned by the solve functions of this algorithm
      */
      typedef SmoInfo output_type;
        
      /**
       * Constructor
       */  
      SmoBase(SmoControl& data);
      
      
      /**
       * @brief Print the alpha coefficients. This function is here mainly for
       * debugging purposes
       */
      std::ostream& print_alphas(std::ostream& out)const;
      
      
protected:
    
     /**
       * @brief The data used to run the algorithm 
       */
      SmoControl data_;
        
      /**
        * @brief The alpha coefficients
        */
      std::vector<real_type> alphas_; 
        
      /**
         * @brief The beta coefficient
         */
      real_type b_;
        
        
      /**
        * @brief The error cache
        */
      std::map<size_type,real_type> error_cache_;
    
    
};


/**
 * Serial Smo algorithm
 */
template<>
class Smo<serial>: protected SmoBase
{
    
    
    public:
        
        /**
         * @brief The output returned by the solve functions of this algorithm
         */
        typedef SmoBase::output_type output_type;
        
        /**
         * @brief Expose functions from the base class that should be public 
         */
        using SmoBase::print_alphas;
        
        
        /**
         * @brief Constructor
         */
        Smo(SmoControl& data);
        
        
        template<typename MatType,typename VecType,typename KernelType>
        SmoInfo solve(const MatType& mat,const VecType& v, 
                   const KernelType& kernel);
        
        
        template<typename MatType,typename VecType,typename KernelType>
        SmoInfo solve(const MatType* mat,const VecType* v, 
                   const KernelType* kernel){
            
            return solve(*mat,*v,*kernel);
        }
          
    protected:
        
        /**
         *@brief Function that calculates the following quantity
         * \sum_{i=1}^N a_iy_i\mathbf{x}_i \cdot \mathbf{x} + beta 
         */
        template<typename MatType,typename LabelsType,typename DataPoint>
        real_type calculate_fxi(const MatType& data,const LabelsType& labels,const DataPoint& point)const;
        
        /**
         * @brief Executes the inner loop of the SMO algorithm
         */
        template<typename MatType,typename LabelsType,typename KernelType>
        size_type inner_loop(const MatType& data,const LabelsType& labels,
                             const KernelType& kernel,size_type i);
        
        /**
         * @brief Update the error cache for the given
         */
        template<typename MatType,typename LabelsType>
        void update_error_cache(const MatType& data,const LabelsType& labels,size_type i);
        
        /**
         * @brief Select the index of the the inner loop alpha. It returns
         * the index of alpha and the error that corresponds to this choice
         */
        template<typename MatType,typename LabelsType>
        std::pair<size_type,real_type> select_J(const MatType& data,const LabelsType& labels,
                                                size_type i,real_type Ei);
               
};

template<typename MatType,typename LabelsType,typename DataPoint>
real_type 
Smo<serial>::calculate_fxi(const MatType& data,const LabelsType& labels,const DataPoint& point)const{
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    real_type rslt = zero;
    
    const size_type m = data_adaptor::size(point);
    
    for(size_type i=0; i<m; ++i){
        
        real_type label_i = static_cast<real_type>(data_adaptor::data_point_val(labels,i));
        
        //most of alphas will be zero therefore do not contribute
        rslt += alphas_[i]*label_i*utilities::maths::inner_product(data_adaptor::data_point(data,i),point,zero);
    }
    
    return rslt + b_;
}

template<typename MatType,typename LabelsType>
void 
Smo<serial>::update_error_cache(const MatType& data,const LabelsType& labels,size_type i){
    
    real_type fxi = calculate_fxi(data,labels,data_adaptor::data_point(data,i));
    const int label_i = data_adaptor::data_point_val(labels,i);
    real_type Ei = fxi - static_cast<real_type>(label_i);
    
    //update the error cache
    utilities::add_or_update_map(error_cache_,i,Ei);    
}

template<typename MatType,typename LabelsType>
std::pair<size_type,real_type> 
Smo<serial>::select_J(const MatType& data,const LabelsType& labels,size_type i,real_type Ei){
    
    size_type max_j = ParML::iuint();
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    real_type max_DE = zero;
    real_type Ej = zero;
    
    //update the error cache
    utilities::add_or_update_map(this->SmoBase::error_cache_,i,Ei);
    
    std::vector<size_type> non_zero_indices;
    
    //get the non zero indices
    smo_detail::get_non_zero_indices(this->SmoBase::error_cache_,non_zero_indices,
                                     this->SmoBase::data_.small_value);
    
    if((!non_zero_indices.empty() && non_zero_indices.size() > 1) ){
        
        //std::cout<<"We have non zero indices..."<<std::endl;
        
        //if we have indices
        for(size_type k=0; k < non_zero_indices.size(); ++k){
            
            size_type idx = non_zero_indices[k];
            
            if(idx == i){
                continue;
            }
            
            real_type fxk = calculate_fxi(data,labels,data_adaptor::data_point(data,idx));
            const int label_k = data_adaptor::data_point_val(labels,idx);
            real_type Ek = fxk - static_cast<real_type>(label_k);
            
            real_type deltaE = std::abs(Ei-Ek);
            
            //std::cout<<" DE: "<<deltaE<<" max_DE "<<max_DE<<std::endl;
            
            if(deltaE >= max_DE){
                
                max_j = idx;
                max_DE = deltaE;
                Ej = Ek;   
            }   
        }
        
        return std::make_pair(max_j,Ej);    
    }

    //otherwise we select randomly
    std::mt19937_64 generator;
    const size_type m = data_adaptor::size(data);
    std::uniform_int_distribution<size_type> distribution(0,m);
    
    size_type idx = i;  
    
    while(idx == i){
        
        idx = distribution(generator);
        
        if(idx == m){
            
            idx -= 1;
        }   
    }
    real_type fxj = calculate_fxi(data,labels,data_adaptor::data_point(data,idx));
    const int label_j = data_adaptor::data_point_val(labels,idx);
    Ej = fxj - static_cast<real_type>(label_j);
    
    return std::make_pair(idx,Ej); 
    
    
}

template<typename MatType,typename LabelsType,typename KernelType>
size_type 
Smo<serial>::inner_loop(const MatType& data,const LabelsType& labels,const KernelType& kernel,size_type i){
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    const int label_i = data_adaptor::data_point_val(labels,i);
    
    //calculate what the models predicts for the i-th data point
    real_type fxi = calculate_fxi(data,labels,data_adaptor::data_point(data,i));
    
    //the error in the prediction
    real_type Ei = fxi - static_cast<real_type>(label_i);
    
    if( ((label_i*Ei < -this->SmoBase::data_.tolerance) && (this->SmoBase::alphas_[i] < data_.C)) ||
        ((label_i*Ei > this->SmoBase::data_.tolerance)  && (this->SmoBase::alphas_[i] > 0)) ){
        
            std::pair<size_type, real_type> Ej_j = select_J(data,labels,i,Ei);
            
            size_type j  = Ej_j.first;
            real_type Ej = Ej_j.second;
            
#ifdef PARML_DEBUG
            using utilities::ExeInvalidIndex;
            std::string msg = "Index: "+std::to_string(i)+" not in [0,"+ std::to_string(this->SmoBase::alphas_.size())+ ")";
            Assert(i < this->SmoBase::alphas_.size(), ExeInvalidIndex(msg));
            msg = "Index: "+std::to_string(j)+" not in [0," + std::to_string(this->SmoBase::alphas_.size())+ ")";
            Assert(j < this->SmoBase::alphas_.size(), ExeInvalidIndex(msg));
#endif
            real_type alphaIold = this->SmoBase::alphas_[i]; 
            real_type alphaJold = this->SmoBase::alphas_[j];
            
            const int label_j = data_adaptor::data_point_val(labels,j);
            
            real_type H = zero;
            real_type L = zero;
            
            smo_detail::determine_H_and_L(H,L,this->SmoBase::data_.C,this->SmoBase::alphas_[i],
                                          this->SmoBase::alphas_[j],label_i,label_j);
             
            if( L == H ){
                std::cout<<"L==H "<<std::endl; 
                return 0;
            }
            
            real_type kx1x2 = kernel(data_adaptor::data_point(data,i),data_adaptor::data_point(data,j));
            real_type kx1x1 = kernel(data_adaptor::data_point(data,i),data_adaptor::data_point(data,i));
            real_type kx2x2 = kernel(data_adaptor::data_point(data,j),data_adaptor::data_point(data,j));
            
            real_type eta = 2.0*kx1x2 - kx1x1 - kx2x2; 
            
            if( eta >= 0){
                std::cout<<" eta>=0 "<<std::endl; 
                return 0;
            }
            
            this->SmoBase::alphas_[j] -= label_j*(Ei-Ej)/eta;
            smo_detail::clip(this->SmoBase::alphas_[j],H,L);
            update_error_cache(data,labels,j);
            
            if (std::abs(this->SmoBase::alphas_[j] - alphaJold) < 0.00001){
                
                std::cout<< "j not moving enough"<<std::endl; 
                return 0;
            }
            
            this->SmoBase::alphas_[i] += label_j*label_i*(alphaJold - this->SmoBase::alphas_[j]);
            update_error_cache(data,labels,i);
            
            real_type b1 = b_ - Ei- label_i*(this->SmoBase::alphas_[i] - alphaIold)*kx1x1 - 
                                    label_j*(this->SmoBase::alphas_[j] - alphaJold)*kx1x2;
                               
                
            real_type b2 = b_ - Ej- label_i*(this->SmoBase::alphas_[i] - alphaIold)*kx1x2 - 
                                    label_j*(this->SmoBase::alphas_[j] - alphaJold)*kx2x2;
            
            smo_detail::determine_beta_coeff(this->SmoBase::b_,this->SmoBase::data_.C,this->SmoBase::alphas_[i],
                                             this->SmoBase::alphas_[j],b1,b2);
               
            return 1;

    }
    
    return 0;
}

template<typename MatType,typename VecType,typename KernelType>
SmoInfo 
Smo<serial>::solve(const MatType& mat,const VecType& v, 
                   const KernelType& kernel){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    SmoInfo info;
    
    //start the iterations
    const size_type max_itrs = this->SmoBase::data_.max_n_iterations;
    
    const size_type m = data_adaptor::size(mat);
    
    const real_type tol = this->SmoBase::data_.tolerance;
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
 
    //resize the alpha coefficients
    this->SmoBase::alphas_.resize(m,zero);
    
    //the beta coefficient
    this->SmoBase::b_ = zero;
    
    size_type alpha_pairs_changed = 0;
    bool entireSet = true;
    
    size_type itr_count = 0;
    
    while( (itr_count < max_itrs) && (alpha_pairs_changed > 0 || entireSet)){
        
        if(this->SmoBase::data_.show_iterations){
            
            std::cout<<"SMO iteration: "<<itr_count<<std::endl;
        }
        
        alpha_pairs_changed = 0;
        
        
        //loop over the the training data
        for(size_type i=0; i<m; ++i){
            
            alpha_pairs_changed += inner_loop(mat,v,kernel,i);
            itr_count++;
                
        }//loop over data points
        
        if(entireSet)
            entireSet = false;
        else if(alpha_pairs_changed == 0)
            entireSet = true;
    }
    
    end = std::chrono::system_clock::now();
    info.niterations = itr_count;
    info.converged = true;
    info.runtime = end-start;
    
   return info; 
    
}



/**
 * Specialization of SMO for multicores
 */
template<>
class Smo<cput_c11>: protected SmoBase,
                     protected par_indexed_object<cput_c11>
{
    
    public:
        
       /**
         * @brief The output returned by the solve functions of this algorithm
         */
        typedef SmoBase::output_type output_type;
        
        
        /**
         * @brief Expose functions from the base class that should be public 
         */
        using SmoBase::print_alphas;
        
        /**
        * @brief Expose functions from par_indexed_object base class
        */
        using par_indexed_object<cput_c11>::clear_partitions;
        using par_indexed_object<cput_c11>::has_partitions;
        using par_indexed_object<cput_c11>::n_partitions;
        using par_indexed_object<cput_c11>::set_partitions;
        using par_indexed_object<cput_c11>::get_partition_id;
        using par_indexed_object<cput_c11>::get_global_to_local_index;
        using par_indexed_object<cput_c11>::get_partition_size;
        using par_indexed_object<cput_c11>::get_partition;
        using par_indexed_object<cput_c11>::get_partition_limits;
        
        /**
         * @brief Constructor
         */
        Smo(SmoControl& data);
        
        
        template<typename MatType,typename VecType,typename KernelType>
        SmoInfo solve(const MatType& mat,const VecType& v, 
                      const KernelType& kernel);
        
        
        template<typename MatType,typename VecType,typename KernelType>
        SmoInfo solve(const MatType* mat,const VecType* v, 
                      const KernelType* kernel){
            
            return solve(*mat,*v,*kernel);
        }
        
        
        /**
        * @brief Partition the given range 
        */
        template<typename PartFun>
        void partition(const utilities::range1d<size_type>& range,
                       const PartFun& partitioner);
        
        
    protected:
         
        /**
         *@brief Function that calculates the following quantity
         * \sum_{i=1}^N a_iy_i\mathbf{x}_i \cdot \mathbf{x} + beta 
         */
        template<typename DataPoint,typename SumOp>
        real_type calculate_fxi(const DataPoint& point,
                                SumOp& sum_op,
                                std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                                                           SumOp> >& sum_state)const;
        
        /**
         * @brief Executes the inner loop of the SMO algorithm
         */
        template<typename MatType,typename LabelsType,
                 typename KernelType,typename SumOp>
        size_type inner_loop(const MatType& data,const LabelsType& labels,
                             const KernelType& kernel,
                             SumOp& sum_op,
                             std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                             SumOp> >& sum_state,
                                             size_type i);
        
        /**
         * @brief Update the error cache for the given
         */
        template<typename MatType,typename LabelsType,typename SumOp>
        void update_error_cache(const MatType& data,const LabelsType& labels,
                                SumOp& sum_op,
                                std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                                 SumOp> >& sum_state,size_type i);
        
        /**
         * @brief Select the index of the the inner loop alpha. It returns
         * the index of alpha and the error that corresponds to this choice
         */
        template<typename MatType,typename LabelsType,typename SumOp>
        std::pair<size_type,real_type> select_J(const MatType& data,const LabelsType& labels,
                                                size_type i,real_type Ei,
                                                SumOp& sum_op,
                                                std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                                                        SumOp > >& sum_state);
               
};

template<typename PartFun>
void 
Smo<cput_c11>::partition(const utilities::range1d<size_type>& range,const PartFun& partitioner){
    
    //how many threads to use
    size_type nthreads = ParML::n_threads();
    this->par_indexed_object<cput_c11>::partition(nthreads,range,partitioner);   
}


template<typename DataPoint,typename SumOp>
real_type 
Smo<cput_c11>::calculate_fxi(const DataPoint& point,
                             SumOp& sum_op,
                             std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                                                        SumOp > >& sum_state)const{
    //reset the point over the sum operation is working
    sum_op.set_data_point(point);
    
    //call reduce from parframepp
    parframepp::ReduceOp<cput_c11>::reduce(this->partitions_,sum_op,sum_state);
    
    real_type rslt = sum_op.get_local_result() + this->SmoBase::b_; 
    
    return rslt;   
}


template<typename MatType,typename LabelsType,typename SumOp>
void 
Smo<cput_c11>::update_error_cache(const MatType& data,const LabelsType& labels,
                                  SumOp& sum_op,
                                  std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                                                        SumOp > >& sum_state,
                                  size_type i){
    
    real_type fxi = calculate_fxi(/*data,labels,*/data_adaptor::data_point(data,i),sum_op,sum_state);
    const int label_i = data_adaptor::data_point_val(labels,i);
    real_type Ei = fxi - static_cast<real_type>(label_i);
    
    //update the error cache
    utilities::add_or_update_map(error_cache_,i,Ei);     
}

template<typename MatType,typename LabelsType,typename SumOp>
std::pair<size_type,real_type> 
Smo<cput_c11>::select_J(const MatType& data,const LabelsType& labels,
                        size_type i,real_type Ei,
                        SumOp& sum_op,
                        std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                                                        SumOp > >& sum_state){
    
    
    
    size_type max_j = ParML::iuint();
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    real_type max_DE = zero;
    real_type Ej = zero;
    
    //update the error cache
    utilities::add_or_update_map(this->SmoBase::error_cache_,i,Ei);
    
    std::vector<size_type> non_zero_indices;
    
    //get the non zero indices
    smo_detail::get_non_zero_indices(this->SmoBase::error_cache_,non_zero_indices,
                                     this->SmoBase::data_.small_value);
    
    if((!non_zero_indices.empty() && non_zero_indices.size() > 1) ){
        
        //std::cout<<"We have non zero indices..."<<std::endl;
        
        //if we have indices
        for(size_type k=0; k < non_zero_indices.size(); ++k){
            
            size_type idx = non_zero_indices[k];
            
            if(idx == i){
                continue;
            }
            
            real_type fxk = calculate_fxi(/*data,labels,*/data_adaptor::data_point(data,idx),sum_op,sum_state);
            const int label_k = data_adaptor::data_point_val(labels,idx);
            real_type Ek = fxk - static_cast<real_type>(label_k);
            
            real_type deltaE = std::abs(Ei-Ek);
            
            //std::cout<<" DE: "<<deltaE<<" max_DE "<<max_DE<<std::endl;
            
            if(deltaE >= max_DE){
                
                max_j = idx;
                max_DE = deltaE;
                Ej = Ek;   
            }   
        }
        
        return std::make_pair(max_j,Ej);    
    }

    //otherwise we select randomly
    std::mt19937_64 generator;
    const size_type m = data_adaptor::size(data);
    std::uniform_int_distribution<size_type> distribution(0,m);
    
    size_type idx = i;  
    
    while(idx == i){
        
        idx = distribution(generator);
        
        if(idx == m){
            
            idx -= 1;
        }   
    }
    real_type fxj = calculate_fxi(/*data,labels,*/data_adaptor::data_point(data,idx),sum_op,sum_state);
    const int label_j = data_adaptor::data_point_val(labels,idx);
    Ej = fxj - static_cast<real_type>(label_j);
    
    return std::make_pair(idx,Ej); 
}

template<typename MatType,typename LabelsType,
         typename KernelType,typename SumOp>
size_type 
Smo<cput_c11>::inner_loop(const MatType& data,
                          const LabelsType& labels,
                          const KernelType& kernel,
                          SumOp& sum_op,
                          std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                                             SumOp > >& sum_state,
                        size_type i){
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    const int label_i = data_adaptor::data_point_val(labels,i);
    
    //calculate what the models predicts for the i-th data point
    real_type fxi = calculate_fxi(/*data,labels,*/data_adaptor::data_point(data,i),sum_op,sum_state);
    
    //the error in the prediction
    real_type Ei = fxi - static_cast<real_type>(label_i);
    
   
    if( ((label_i*Ei < -this->SmoBase::data_.tolerance) && (this->SmoBase::alphas_[i] < this->SmoBase::data_.C)) ||
        ((label_i*Ei > this->SmoBase::data_.tolerance)  && (this->SmoBase::alphas_[i] > 0)) ){
        
            std::pair<size_type, real_type> Ej_j = select_J(data,labels,i,Ei,sum_op,sum_state);
            
            size_type j  = Ej_j.first;
            real_type Ej = Ej_j.second;
            
#ifdef PARML_DEBUG
            using utilities::ExeInvalidIndex;
            std::string msg = "Index: "+std::to_string(i)+" not in [0,"+ std::to_string(this->SmoBase::alphas_.size())+ ")";
            Assert(i < this->SmoBase::alphas_.size(), ExeInvalidIndex(msg));
            msg = "Index: "+std::to_string(j)+" not in [0," + std::to_string(this->SmoBase::alphas_.size())+ ")";
            Assert(j < this->SmoBase::alphas_.size(), ExeInvalidIndex(msg));
#endif
            real_type alphaIold = this->SmoBase::alphas_[i]; 
            real_type alphaJold = this->SmoBase::alphas_[j];
            
            const int label_j = data_adaptor::data_point_val(labels,j);
            
            real_type H = zero;
            real_type L = zero;
            
            smo_detail::determine_H_and_L(H,L,this->SmoBase::data_.C,this->SmoBase::alphas_[i],
                                          this->SmoBase::alphas_[j],label_i,label_j);
             
            if( L == H ){
                std::cout<<"L==H "<<std::endl; 
                return 0;
            }
            
            real_type kx1x2 = kernel(data_adaptor::data_point(data,i),data_adaptor::data_point(data,j));
            real_type kx1x1 = kernel(data_adaptor::data_point(data,i),data_adaptor::data_point(data,i));
            real_type kx2x2 = kernel(data_adaptor::data_point(data,j),data_adaptor::data_point(data,j));
            
            real_type eta = 2.0*kx1x2 - kx1x1 - kx2x2; 
            
            if( eta >= 0){
                std::cout<<" eta>=0 "<<std::endl; 
                return 0;
            }
            
            this->SmoBase::alphas_[j] -= label_j*(Ei-Ej)/eta;
            
            smo_detail::clip(this->SmoBase::alphas_[j],H,L);
            
            update_error_cache(data,labels,sum_op,sum_state,j);
            
            if (std::abs(this->SmoBase::alphas_[j] - alphaJold) < 0.00001){
                
                std::cout<< "j not moving enough"<<std::endl; 
                return 0;
            }
            
            this->SmoBase::alphas_[i] += label_j*label_i*(alphaJold - this->SmoBase::alphas_[j]);
            
            update_error_cache(data,labels,sum_op,sum_state,i);
            
            real_type b1 = b_ - Ei- label_i*(this->SmoBase::alphas_[i] - alphaIold)*kx1x1 - 
                                    label_j*(this->SmoBase::alphas_[j] - alphaJold)*kx1x2;
                               
                
            real_type b2 = b_ - Ej- label_i*(this->SmoBase::alphas_[i] - alphaIold)*kx1x2 - 
                                    label_j*(this->SmoBase::alphas_[j] - alphaJold)*kx2x2;
            
            smo_detail::determine_beta_coeff(this->SmoBase::b_,
                                             this->SmoBase::data_.C,
                                             this->SmoBase::alphas_[i],
                                             this->SmoBase::alphas_[j],b1,b2);  
            return 1;
    }
    
    return 0;
}


template<typename MatType,typename LabelsType,typename KernelType>
SmoInfo 
Smo<cput_c11>::solve(const MatType& mat,const LabelsType& v, 
                     const KernelType& kernel){
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    
//if we don't have partitions then signal an error
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = "Partitions for parallel object are empty";
    Assert(this->has_partitions()==true,ExeLogicError(msg));
#endif
    
    SmoInfo info;
    info.nthreads = ParML::n_threads();
    
    //start the iterations
    const size_type max_itrs = data_.max_n_iterations; 
    
    const size_type m = data_adaptor::size(mat);
    
    const real_type tol = data_.tolerance;
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    
    //resize the alpha coefficients
    this->SmoBase::alphas_.resize(m,zero);
    
    //the beta coefficient
    this->SmoBase::b_ = zero;
    
    typedef typename data_point_type<MatType>::row_type DataPointType;
    
    //the object that holds the partial sum
    par_smo_detail::partial_fxi_task<MatType,LabelsType,
                                     DataPointType,KernelType> partial_fxi(mat,v,kernel,this->SmoBase::alphas_);
       
       
    std::unique_ptr<parframepp::ReductionState<utilities::range1d<size_type>,
                    par_smo_detail::partial_fxi_task<MatType,LabelsType,DataPointType,KernelType> > > partial_fxi_state; 
       
    
    
    size_type alpha_pairs_changed = 0;
    bool entireSet = true;
    
    size_type itr_count = 0;
    
    while( (itr_count < max_itrs) && (alpha_pairs_changed > 0 || entireSet)){
        
        if(this->SmoBase::data_.show_iterations){
            
            std::cout<<"SMO iteration: "<<itr_count<<std::endl;
        }
        
        alpha_pairs_changed = 0;
        
        
        //loop over the the training data
        for(size_type i=0; i<m; ++i){
            
            alpha_pairs_changed += inner_loop(mat,v,kernel,partial_fxi,partial_fxi_state,i);
            itr_count++;
                
        }//loop over data points
        
        if(entireSet)
            entireSet = false;
        else if(alpha_pairs_changed == 0)
            entireSet = true;
    }
    
    end = std::chrono::system_clock::now();
    info.niterations = itr_count;
    info.converged = true;
    info.runtime = end-start;
    
   return info; 
    
}

#ifdef PARML_HAS_MPI

template<>
class Smo<mpi_par>: protected SmoBase,
                    protected par_indexed_object<mpi_par>
{
    
    public:
        
       /**
         * @brief The output returned by the solve functions of this algorithm
         */
        typedef SmoBase::output_type output_type;
        
        
        /**
         * @brief Expose functions from the base class that should be public 
         */
        using SmoBase::print_alphas;
        
        /**
        * @brief Expose functions from par_indexed_object base class
        */
        using par_indexed_object<mpi_par>::clear_partitions;
        using par_indexed_object<mpi_par>::has_partitions;
        using par_indexed_object<mpi_par>::n_partitions;
        using par_indexed_object<mpi_par>::set_partitions;
        using par_indexed_object<mpi_par>::get_partition_id;
        using par_indexed_object<mpi_par>::get_global_to_local_index;
        using par_indexed_object<mpi_par>::get_partition_size;
        using par_indexed_object<mpi_par>::get_partition;
        using par_indexed_object<mpi_par>::get_partition_limits;
        using par_indexed_object<mpi_par>::get_partitions;
        
        /**
         * @brief Constructor
         */
        Smo(SmoControl& data);
        
        
        template<typename MatType,typename VecType,typename KernelType>
        output_type solve(const MatType& mat,const VecType& v, 
                          const KernelType& kernel);
        
        
        template<typename MatType,typename VecType,typename KernelType>
        output_type solve(const MatType* mat,const VecType* v, 
                      const KernelType* kernel){
            
            return solve(*mat,*v,*kernel);
        }
        
        
        /**
        * @brief Partition the given range 
        */
        template<typename PartFun>
        void partition(const utilities::range1d<size_type>& range,
                       const PartFun& partitioner);
        
        
    protected:
         
        /**
         *@brief Function that calculates the following quantity
         * \sum_{i=1}^N a_iy_i\mathbf{x}_i \cdot \mathbf{x} + beta 
         */
        template<typename DataPoint,typename SumOp>
        real_type calculate_fxi(const DataPoint& point,SumOp& sum_op)const;
        
        /**
         * @brief Executes the inner loop of the SMO algorithm
         */
        template<typename MatType,typename LabelsType,
                 typename KernelType,typename SumOp>
        size_type inner_loop(const MatType& data,const LabelsType& labels,
                             const KernelType& kernel,
                             SumOp& sum_op,size_type i);
        
        /**
         * @brief Update the error cache for the given
         */
        template<typename MatType,typename LabelsType,typename SumOp>
        void update_error_cache(const MatType& data,const LabelsType& labels,
                                SumOp& sum_op,size_type i);
        
        /**
         * @brief Select the index of the the inner loop alpha. It returns
         * the index of alpha and the error that corresponds to this choice
         */
        template<typename MatType,typename LabelsType,typename SumOp>
        std::pair<size_type,real_type> select_J(const MatType& data,
                                                const LabelsType& labels,
                                                const std::vector<size_type>& non_zero_indices,
                                                size_type i,real_type Ei,
                                                SumOp& sum_op);
        
        
        size_type get_non_zero_indices_on_master(std::vector<size_type>& indices,
                                                 size_type i,real_type Ei);
        
        template<typename MatType,typename DataPointType>
        void communicate_datapoint(const MatType& data,DataPointType& datapoint,
                                    size_type i,size_type part_id)const;
        template<typename LabelsType>
        void communicate_label(const LabelsType& labels,int& label,
                               size_type i,size_type part_id)const;
        
        template<typename MatType,typename DataPointType>
        void communicate_datapoint_to_master(const MatType& data,DataPointType& datapoint,
                                             size_type i,size_type part_id)const;
               
};

template<typename PartFun>
void 
Smo<mpi_par>::partition(const utilities::range1d<size_type>& range,const PartFun& partitioner){
    
    //how many threads to use
    size_type nthreads = ParML::n_threads();
    
    this->par_indexed_object<mpi_par>::partition(nthreads,range,partitioner);   
}


template<typename DataPoint,typename SumOp>
real_type 
Smo<mpi_par>::calculate_fxi(const DataPoint& point,SumOp& sum_op)const{
    
     //reset the point over the sum operation is working
    sum_op.set_data_point(point);
    
    //execute the local operation
    sum_op(this->get_partition(ParML::pid()));
    
    real_type rslt = sum_op.get_local_result(); // + this->SmoBase::b_;
    
    real_type reduction_rslt = utilities::scalar_value_traits<real_type>::zero();
    
    //reduce to the master process
    parframepp::RTBase<mpi_par>::reduce(rslt,reduction_rslt,
                                        parframepp::RTBase<mpi_par>::master_process_id(),std::plus<real_type>());
    
    return reduction_rslt + this->SmoBase::b_;      
}


template<typename MatType,typename LabelsType,typename SumOp>
void Smo<mpi_par>::update_error_cache(const MatType& data,
                                      const LabelsType& labels,
                                      SumOp& sum_op,size_type i){
    
    //for the given point
    typedef typename data_point_type<MatType>::row_type DataPointType;
    DataPointType datapoint;
    int label_i;
    
    
    size_type part_id = this->get_partition_id(i);
    communicate_datapoint(data,datapoint,i,part_id);
    communicate_label(labels,label_i,i,part_id);
    
    //this is valid only on the master process
    real_type fxi = calculate_fxi(datapoint,sum_op);
    
    if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
        
        real_type Ei = fxi - static_cast<real_type>(label_i);
    
        //update the error cache
        utilities::add_or_update_map(this->SmoBase::error_cache_,i,Ei); 
    }
}


template<typename MatType,typename LabelsType,typename SumOp>
std::pair<size_type,real_type> 
Smo<mpi_par>::select_J(const MatType& data,
                       const LabelsType& labels,
                       const std::vector<size_type>& non_zero_indices,
                       size_type i,real_type Ei,
                       SumOp& sum_op){
    
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    typedef typename data_point_type<MatType>::row_type DataPointType;
    DataPointType datapoint;
    int label_k;
    
    if((!non_zero_indices.empty() && non_zero_indices.size() > 1) ){
        
        size_type max_j = ParML::iuint();
        real_type max_DE = zero;
        real_type Ej = zero;
        
        //if we have indices
        for(size_type k=0; k < non_zero_indices.size(); ++k){
            
            size_type idx = non_zero_indices[k];
            
            if(idx == i){
                continue;
            }
            
            //if the index local
            size_type part_id = this->get_partition_id(idx);
            communicate_datapoint(data,datapoint,idx,part_id);
            communicate_label(labels,label_k,idx,part_id);
            
            real_type fxk = calculate_fxi(datapoint,sum_op);
            real_type Ek = fxk - static_cast<real_type>(label_k);
            
            real_type deltaE = std::abs(Ei-Ek);
            
            if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
                if(deltaE >= max_DE){
                
                    max_j = idx;
                    max_DE = deltaE;
                    Ej = Ek;   
                }   
            }
        }
        
        return std::make_pair(max_j,Ej);    
    }

    //otherwise we select randomly
    size_type idx = i; 
    
    if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
        
        std::mt19937_64 generator;
        const size_type m = this->SmoBase::data_.n_training_examples; 
        std::uniform_int_distribution<size_type> distribution(0,m);
    
        while(idx == i){
        
            idx = distribution(generator);
        
            if(idx == m){

                idx -= 1;
            }   
        }
    }
    
    //broadcast the index we found
    parframepp::RTBase<mpi_par>::broadcast(idx,parframepp::RTBase<mpi_par>::master_process_id());
    
    size_type part_id = this->get_partition_id(idx);
    
    communicate_datapoint(data,datapoint,idx,part_id);
    communicate_label(labels,label_k,idx,part_id);
    
    real_type fxj = calculate_fxi(datapoint,sum_op);
    real_type Ej = fxj - static_cast<real_type>(label_k);
    return std::make_pair(idx,Ej);  
}


template<typename MatType,typename DataPointType>
void 
Smo<mpi_par>::communicate_datapoint(const MatType& data,DataPointType& datapoint,
                                    size_type i,size_type part_id)const{
    
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = "Partition id: "+std::to_string(part_id)+
                            "not in [0,"+std::to_string(ParML::n_processors())+")";
    Assert(part_id != ParML::iuint(),ExeLogicError(msg));
#endif 
    
    if(ParML::pid() == part_id){
        
        size_type local_index = this->get_global_to_local_index(part_id,i);
        datapoint = data_adaptor::data_point(data,local_index);
    }
    else{
      
        datapoint.resize(1,this->SmoBase::data_.n_features);   
    }
    
    //brodcast the datapoint
    real_type* data_ptr = datapoint.data();
     
    //broadcast the point
    parframepp::RTBase<mpi_par>::broadcast(data_ptr,this->SmoBase::data_.n_features,part_id);
}

template<typename MatType,typename DataPointType>
void 
Smo<mpi_par>::communicate_datapoint_to_master(const MatType& data,DataPointType& datapoint,
                                              size_type i,size_type part_id)const{
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = "Partition id: "+std::to_string(part_id)+
                            "not in [0,"+std::to_string(ParML::n_processors())+")";
    Assert(part_id != ParML::iuint(),ExeLogicError(msg));
#endif 
    
    size_type send_data_point_tag = 2;
    
    if(ParML::pid() == part_id){
        
        size_type local_index = this->get_global_to_local_index(part_id,i);
        datapoint = data_adaptor::data_point(data,local_index);
        
        if(ParML::pid() != parframepp::RTBase<mpi_par>::master_process_id() ){
            
            real_type* data = datapoint.data();
            
            parframepp::RTBase<mpi_par>::send(data,
                                              this->SmoBase::data_.n_features,
                                              parframepp::RTBase<mpi_par>::master_process_id(),send_data_point_tag );
        }
    }
    else if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
        
        datapoint.resize(1,this->SmoBase::data_.n_features); 
        real_type* data = datapoint.data();
            
        parframepp::RTBase<mpi_par>::receive(data,
                                              this->SmoBase::data_.n_features,
                                              part_id,send_data_point_tag );
    }    
}

template<typename LabelsType>
void 
Smo<mpi_par>::communicate_label(const LabelsType& labels,int& label,
                                size_type i,size_type part_id)const{
    
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = "Partition id: "+std::to_string(part_id)+
                            "not in [0,"+std::to_string(ParML::n_processors())+")";
    Assert(part_id != ParML::iuint(),ExeLogicError(msg));
#endif 
    
    if(ParML::pid() == part_id){
        size_type local_index = this->get_global_to_local_index(part_id,i);
        label = data_adaptor::data_point_val(labels,local_index);    
    }
    
    //broadcast the label
    parframepp::RTBase<mpi_par>::broadcast(label,part_id);
    
}

template<typename MatType,typename LabelsType,
         typename KernelType,typename SumOp>
size_type 
Smo<mpi_par>::inner_loop(const MatType& data,
                          const LabelsType& labels,
                          const KernelType& kernel,
                          SumOp& sum_op,
                          size_type i){
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    
    //find out to which processor the i-th datapoint belongs to
    //the processor that holds the point communicates it
    //to the rest of the processors to do work
    size_type part_id = this->get_partition_id(i);
       
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg = "Partition id: "+std::to_string(part_id)+
                            "not in [0,"+std::to_string(ParML::n_processors())+")";
    Assert(part_id != ParML::iuint(),ExeLogicError(msg));
#endif

    
    typedef typename data_point_type<MatType>::row_type DataPointType;
    
    DataPointType datapoint;
    int label_i;
    
    communicate_datapoint(data,datapoint,i,part_id);
    communicate_label(labels,label_i,i,part_id);
    
    //calculate what the models predicts for the i-th data point
    real_type fxi = calculate_fxi(datapoint,sum_op);
    
    //the error in the prediction. This is only valid
    //on the master process
    real_type Ei = fxi - static_cast<real_type>(label_i);
    
    //the non zero indices
    std::vector<size_type> non_zero_indices;
    
    //dummy flag indicating that indices are to be
    //sent
    size_type indices_to_be_sent = 0;
    
    if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
   
        if( ((label_i*Ei < -this->SmoBase::data_.tolerance) && 
             (this->SmoBase::alphas_[i] < this->SmoBase::data_.C)) ||
            ((label_i*Ei > this->SmoBase::data_.tolerance)  && 
             (this->SmoBase::alphas_[i] > 0)) ){
            
            get_non_zero_indices_on_master(non_zero_indices,i,Ei);
            indices_to_be_sent = non_zero_indices.size();
            
            //std::cout<<"On master process indices to be sent: "<<indices_to_be_sent<<std::endl<<std::flush;
        }
    }
    
    //let the master communicate the non-zero indices
    parframepp::RTBase<mpi_par>::broadcast(indices_to_be_sent,parframepp::RTBase<mpi_par>::master_process_id());
    
    if(indices_to_be_sent > 1){
        
        //let's communicate the indices
         //parframepp::RTRoot::Info<<"Get into Process pid: "<<ParML::pid()<<std::endl<<std::flush;
         
         int all_procs_exit = -1;
        
        if(ParML::pid() != parframepp::RTBase<mpi_par>::master_process_id()){
            non_zero_indices.resize(indices_to_be_sent);
        }
        
        parframepp::RTBase<mpi_par>::broadcast(&non_zero_indices[0],
                                               indices_to_be_sent,
                                               parframepp::RTBase<mpi_par>::master_process_id()); 
        
        //we got the indices now let's select J
        std::pair<size_type, real_type> Ej_j = select_J(data,labels,non_zero_indices,i,Ei,sum_op);
        
        int label_j;
        size_type j;
        real_type Ej;
        
        real_type alphaIold; 
        real_type alphaJold;
        
        //only the master process works here
        if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
            
            j  = Ej_j.first;
            Ej = Ej_j.second;
            
#ifdef PARML_DEBUG
            using utilities::ExeInvalidIndex;
            std::string msg = "Index: "+std::to_string(i)+" not in [0,"+ 
                              std::to_string(this->SmoBase::alphas_.size())+ ")";
            Assert(i < this->SmoBase::alphas_.size(), ExeInvalidIndex(msg));
            msg = "Index: "+std::to_string(j)+" not in [0," + std::to_string(this->SmoBase::alphas_.size())+ ")";
            Assert(j < this->SmoBase::alphas_.size(), ExeInvalidIndex(msg));
#endif
            alphaIold = this->SmoBase::alphas_[i]; 
            alphaJold = this->SmoBase::alphas_[j];   
        }
        
        //the master process broadcasts the j index
        parframepp::RTBase<mpi_par>::broadcast(j,parframepp::RTBase<mpi_par>::master_process_id());
        
        size_type send_label_tag = 1;
        
        size_type label_j_pid = this->get_partition_id(j);
        
        //now let's find out the label
        if(label_j_pid == ParML::pid()){
            
            //send the label
            int local_label_idx = this->get_global_to_local_index(ParML::pid(),j);
            label_j = data_adaptor::data_point_val(labels,local_label_idx);
            
            if(ParML::pid() != parframepp::RTBase<mpi_par>::master_process_id()){
                
                parframepp::RTBase<mpi_par>::send(&label_j,
                                                  static_cast<size_type>(1),
                                                  parframepp::RTBase<mpi_par>::master_process_id(),
                                                  send_label_tag);   
            }
        }
        else if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
            
            parframepp::RTBase<mpi_par>::receive(&label_j,
                                                 static_cast<size_type>(1),
                                                 label_j_pid,
                                                 send_label_tag);
        }
            
        real_type H;
        real_type L;
            
        if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
            
            H = zero;
            L = zero;
            
            smo_detail::determine_H_and_L(H,L,this->SmoBase::data_.C,
                                          this->SmoBase::alphas_[i],
                                          this->SmoBase::alphas_[j],
                                            label_i,label_j);
             
            if( L == H ){
                std::cout<<"L==H "<<std::endl; 
                all_procs_exit = 1;
            }
        }
        
        parframepp::RTBase<mpi_par>::broadcast(all_procs_exit,parframepp::RTBase<mpi_par>::master_process_id());
        
        if(all_procs_exit == 1)
            return 0;
        
        //we need to get the off processor point
        
        DataPointType datapoint_i;
        DataPointType datapoint_j;
        size_type dp_i_pid = this->get_partition_id(i);
        
        communicate_datapoint_to_master(data,datapoint_i,i,dp_i_pid);
        communicate_datapoint_to_master(data,datapoint_j,j,label_j_pid);
        
        real_type kx1x2; 
        real_type kx1x1;
        real_type kx2x2;
        
        if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
            
            real_type kx1x2 = kernel(datapoint_i,datapoint_j);
            real_type kx1x1 = kernel(datapoint_i,datapoint_i);
            real_type kx2x2 = kernel(datapoint_j,datapoint_j);
            
            real_type eta = 2.0*kx1x2 - kx1x1 - kx2x2; 
            
            if( eta >= 0){
                std::cout<<" eta>=0 "<<std::endl; 
                all_procs_exit = 1;
            }
            
            if(all_procs_exit != 1){
                this->SmoBase::alphas_[j] -= label_j*(Ei-Ej)/eta;
                smo_detail::clip(this->SmoBase::alphas_[j],H,L);
            }
        }
        
        parframepp::RTBase<mpi_par>::broadcast(all_procs_exit,parframepp::RTBase<mpi_par>::master_process_id());
        
        if(all_procs_exit == 1)
            return 0;
        
        //std::cout<<"Process pid: "<<ParML::pid()<<std::endl<<std::flush;
        update_error_cache(data,labels,sum_op,j);
            
        if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
            
            if (std::abs(this->SmoBase::alphas_[j] - alphaJold) < 0.00001){
                
                std::cout<< "j not moving enough"<<std::endl; 
                all_procs_exit = 1;
            }
            
             if(all_procs_exit != 1){
                this->SmoBase::alphas_[i] += label_j*label_i*(alphaJold - this->SmoBase::alphas_[j]);
             }
        }
            
        parframepp::RTBase<mpi_par>::broadcast(all_procs_exit,parframepp::RTBase<mpi_par>::master_process_id());
        
        if(all_procs_exit == 1)
            return 0;
        
           update_error_cache(data,labels,sum_op,i);
            
        if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
            
            real_type b1 = b_ - Ei- label_i*(this->SmoBase::alphas_[i] - alphaIold)*kx1x1 - 
                                    label_j*(this->SmoBase::alphas_[j] - alphaJold)*kx1x2;
                                  
            real_type b2 = b_ - Ej- label_i*(this->SmoBase::alphas_[i] - alphaIold)*kx1x2 - 
                                    label_j*(this->SmoBase::alphas_[j] - alphaJold)*kx2x2;
            
            smo_detail::determine_beta_coeff(this->SmoBase::b_,
                                             this->SmoBase::data_.C,
                                             this->SmoBase::alphas_[i],
                                             this->SmoBase::alphas_[j],b1,b2);     
        } 
           
        return 1;
    }//indices_to_be_sent > 1
    
    return 0;
}




template<typename MatType,typename LabelsType,typename KernelType>
typename Smo<mpi_par>::output_type
Smo<mpi_par>::solve(const MatType& mat,const LabelsType& v, const KernelType& kernel){
    
   std::chrono::time_point<std::chrono::system_clock> start, end;
   start = std::chrono::system_clock::now();
    
//if we don't have partitions then signal an error
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    std::string msg = "Partitions for parallel object are empty";
    Assert(this->has_partitions()==true,ExeLogicError(msg));
    msg = "Invalid number of training examples";
    Assert(this->SmoBase::data_.n_training_examples != ParML::iuint(),ExeLogicError(msg));
    msg = "Invalid number of features";
    Assert(this->SmoBase::data_.n_features != ParML::iuint(),ExeLogicError(msg));   
#endif
    
    SmoInfo info;
    
    info.nthreads = ParML::n_threads();
    info.nprocs = ParML::n_processors();
    
    //start the iterations
    const size_type max_itrs = data_.max_n_iterations;
    
    const size_type m = this->SmoBase::data_.n_training_examples; //data_adaptor::size(mat);
    
    const real_type tol = data_.tolerance;
    
    const real_type zero = utilities::scalar_value_traits<real_type>::zero();
    
    //resize the alpha coefficients
    this->SmoBase::alphas_.resize(m,zero);
    
    //the beta coefficient
    this->SmoBase::b_ = zero;
    
    
    typedef typename data_point_type<MatType>::row_type DataPointType;
    
    //the object that holds the partial sum
    par_smo_detail::partial_fxi_task_mpi<MatType,LabelsType,
                                     DataPointType,KernelType> partial_fxi(mat,v,kernel,this->SmoBase::alphas_);
       
    size_type alpha_pairs_changed = 0;
    bool entireSet = true;
     
    size_type itr_count = 0;
    
    while( (itr_count < max_itrs) && (alpha_pairs_changed > 0 || entireSet)){
        
        if(this->SmoBase::data_.show_iterations){
            
            //std::cout<<"SMO iteration: "<<itr_count<<std::endl;
        }
        
        alpha_pairs_changed = 0;
        
        
        //loop over the the training data
        for(size_type i=0; i<m; ++i){
            
            alpha_pairs_changed += inner_loop(mat,v,kernel,partial_fxi,i);
            itr_count++;
                
        }//loop over data points
        
        if(entireSet)
            entireSet = false;
        else if(alpha_pairs_changed == 0)
            entireSet = true;
    }
    
    end = std::chrono::system_clock::now();
    info.niterations = itr_count;
    info.converged = true;
    info.runtime = end-start;
    
   return info; 
    
}

#endif 


    
}//parml



#endif	/* SMO_H */

