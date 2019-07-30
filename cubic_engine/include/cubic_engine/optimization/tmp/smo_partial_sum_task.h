/* 
 * File:   smo_partial_sum_tasks.h
 * Author: david
 *
 * Created on July 25, 2016, 7:56 PM
 */

#ifndef SMO_PARTIAL_SUM_TASK_H
#define	SMO_PARTIAL_SUM_TASK_H

#include "parml/base/parml.h"
#include "parml/base/data_adaptor.h"
#include "parml/base/parml_config.h"

#include "utilities/range1d.h"
#include "utilities/scalar_value_traits.h"

#include <vector>

namespace parml{
    

    namespace par_smo_detail{
    
    
    template<typename MatType,typename LabelsType,
             typename DataPointType,typename KernelType>
    class partial_fxi_task
    {
        
        public:
            
            typedef typename KernelType::value_type value_type;
            typedef value_type local_result_type;
            typedef value_type local_result_return_type;
            
            
            /**
             * @brief Constructor
             */
            partial_fxi_task(const MatType& data,const LabelsType& labels,
                             const KernelType& kernel,const std::vector<real_type>& alphas)
            :
            data_(&data),
            labels_(&labels),
            kernel_(&kernel),
            alphas_(&alphas),
            input_point_(nullptr),
            local_result_(utilities::scalar_value_traits<real_type>::zero())
            {}
            
            /**
             * @brief Return the computed local result;
             */
            local_result_return_type get_local_result()const{return local_result_;}
            
            /**
             * @brief Calculate the local contribution to the global error
             */
            virtual void operator()(const utilities::range1d<size_type>& range);
            
            
            /**
             * @brief Join the local result with the given result
             */
            void join(local_result_return_type other ){local_result_ += other;}
            
            
            /**
             * @brief Zero the local gradients
             */
            void zero_result(){local_result_ = utilities::scalar_value_traits<real_type>::zero();}
            
            
            /**
             * @brief Set the data point over which the partial sum is calculated
             */
            void set_data_point(const DataPointType& datapoint ){input_point_ = &datapoint;}
            
        protected:
            
            /**
             * @brief The matrix object that holds the training set
             */
            const MatType* data_;
            
            /**
             * @brief The labels associated with the data set
             */
            const LabelsType* labels_;
        
            /**
             * @brief the kernel type
             */
            const KernelType* kernel_;
            
            /**
             * @brief The optimization coefficients
             */
            const std::vector<real_type>* alphas_;
            
            
            /**
             * @brief The input point on which the partial sum is calculated
             */
            const DataPointType* input_point_;
            
            /**
             * @brief The local result
             */
            local_result_type local_result_; 
        
    };
    
        template<typename MatType,typename LabelsType,
                 typename DataPointType,typename KernelType>
        void 
        partial_fxi_task<MatType,
                         LabelsType,
                         DataPointType,
                         KernelType>::operator()(const utilities::range1d<size_type>& range){
        
            zero_result();
            size_type begin = range.begin();
            size_type end   = range.end();
        
            for(size_type i=begin; i<end; ++i){
            
                real_type ai = (*alphas_)[i];
                real_type yi = data_adaptor::data_point_val(*labels_,i);
            
                //calculate the kernel
                real_type kernel_val = kernel_->operator()(data_adaptor::data_point(*data_,i),*input_point_);
            
                local_result_ += ai*yi*kernel_val;
            }   
        }
        
#ifdef PARML_HAS_MPI
        template<typename MatType,typename LabelsType,
                typename DataPointType,typename KernelType>
        class partial_fxi_task_mpi: public partial_fxi_task<MatType,
                                                            LabelsType,
                                                            DataPointType,KernelType>
        {
            
            public:
                
                partial_fxi_task_mpi(const MatType& data,const LabelsType& labels,
                                     const KernelType& kernel,
                                     const std::vector<real_type>& alphas)
                        :
                partial_fxi_task<MatType,LabelsType,DataPointType,KernelType>(data,labels,kernel,alphas)
                {}
                
                
                 /**
                   * @brief Calculate the local contribution to the global error
                   */
                virtual void operator()(const utilities::range1d<size_type>& range);
                
        };
        
        template<typename MatType,typename LabelsType,
                 typename DataPointType,typename KernelType>
        void 
        partial_fxi_task_mpi<MatType,
                             LabelsType,
                             DataPointType,
                             KernelType>::operator()(const utilities::range1d<size_type>& range){
        
            this->zero_result();
            size_type begin = range.begin();
            size_type end   = range.end();
        
            for(size_type i=begin; i<end; ++i){
                
                size_type local_idx = parframepp::rt_root::get_global_to_local_index(range,i);
            
                real_type ai = (*this->alphas_)[local_idx];
                real_type yi = data_adaptor::data_point_val(*this->labels_,local_idx);
            
                //calculate the kernel
                real_type kernel_val = this->kernel_->operator()(data_adaptor::data_point(*this->data_,local_idx),
                                                                 *this->input_point_);
            
                this->local_result_ += ai*yi*kernel_val;
            }   
        }
#endif 
    }//par_smo_detail
}//parml


#endif	/* SMO_PARTIAL_SUM_TASK_H */

