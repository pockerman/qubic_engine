/* 
 * File:   batch_gradient_descent_detail.h
 * Author: david
 *
 * Created on July 28, 2016, 10:31 AM
 */

#ifndef PARALLEL_BATCH_GRADIENT_DESCENT_DETAIL_H
#define	PARALLEL_BATCH_GRADIENT_DESCENT_DETAIL_H

#include "parml/base/parml.h"
#include "parml/base/parml_config.h"

#include "utilities/scalar_value_traits.h"
#include "utilities/range1d.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif 

#include "parframepp/base/reduction_object.h"

#include <vector>
#include <algorithm> //std::for_each
#include <functional> //std::plus

namespace parml
{
    
    namespace par_batch_gd_detail
    {

        /**
         * @brief Helper objects used with MPI and C++11 threads.
         */

        /**
         * @brief Class to wrap the calculation
         * of a portion of the error function 
         */
        template<typename J,typename H>
        struct SumOp: public parframepp::reduce_object<typename H::value_type,
                                                         std::plus<typename H::value_type>>
        {
            public:

                typedef typename H::value_type value_type;
                
                
                /**
                 * @brief Constructor
                 */
                SumOp();
                
                
                /**
                 * @brief Constructor
                 */
                SumOp(const J& j,const H& h);
                

                /**
                 * @brief Calculate the local contribution to the global error
                 */
                void operator()(const utilities::range1d<size_type>& range);
                
                /**
                 * @brief Copy the data to other (shallow copy)
                 */
                void copy_to(SumOp<J,H>& other)const;


            private:

                /**
                 * @brief Reference to the error function object
                 */
                const J* error_func_;

                /**
                 * @brief Reference to the hypothesis function
                 */
                const H* h_;
        };
        template<typename J,typename H>
        SumOp<J,H>::SumOp()
        :
        parframepp::reduce_object<value_type,
                                  std::plus<value_type>>(),
        error_func_(nullptr),
        h_(nullptr)
        {}
        
        template<typename J,typename H>
        SumOp<J,H>::SumOp(const J& j,const H& h)
        :
        parframepp::reduce_object<value_type,
                                  std::plus<value_type>>(),
        error_func_(&j),
        h_(&h)
        {}
        
        
        template<typename J,typename H>
        void 
        SumOp<J,H>::operator()(const utilities::range1d<size_type>& range){
#ifdef PARML_DEBUG
            using utilities::ExeLogicError;
            std::string msg = "Null Error Function Pointer";
            Assert(error_func_ != nullptr,ExeLogicError(msg));
            
            msg = "Null Hypothesis Function Pointer";
            Assert(h_ != nullptr,ExeLogicError(msg));
#endif
            this->res_ = error_func_->range_value(range,*h_);
        }
        
        template<typename J,typename H>
        void 
        SumOp<J,H>::copy_to(SumOp<J,H>& other)const{
            
            other.error_func_ = error_func_;
            other.h_ = h_;
        }

        /**
         * @brief Class to wrap the calculation of the gradients
         */
        template<typename J,typename H>
        class GradOp
        {

            public:

                typedef typename H::value_type value_type;
                typedef std::vector<value_type> result_type;
                
                /**
                 * @brief Constructor
                 */
                
                GradOp();
                
                /**
                 * @brief Constructor
                 */
                GradOp(const J& error_func,const H& h);
               

                /**
                 * @brief Return the computed local result;
                 */
                const result_type& get_result()const{return res_;}
                
                /**
                 * @brief Return the computed local result;
                 */
                result_type& get_result(){return res_;}

                /**
                 * @brief Calculate the local contribution to the global error
                 */
                void operator()(const utilities::range1d<size_type>& range);


                /**
                 * @brief Join the local result with the given result
                 */
                void join(const result_type& other );


                /**
                  * @brief Reset the value of the result to the state just after calling the
                  * constructor
                  */
                void reset();
                
                /**
                 * @brief Zero the contents of result container
                 */
                void zero();
                
                /**
                 * @brief Copy the data to other (shallow copy)
                 */
                void copy_to(GradOp<J,H>& other)const;

            private:

                /**
                 * @brief Store the partial result of the i-th gradient of
                 * the error function
                 */
                result_type res_;


                /**
                 * @brief Reference to the error function object
                 */
                const J* error_func_;

                /**
                 * @brief Reference to the hypothesis function
                 */
                const H* h_;    
        };
        
        
        template<typename J,typename H>
        GradOp<J,H>::GradOp()
        :
        res_(),
        error_func_(nullptr),
        h_(nullptr)
        {}
        
        template<typename J,typename H>
        GradOp<J,H>::GradOp(const J& error_func,const H& h)
                        :
        res_(),
        error_func_(&error_func),
        h_(&h)
        {}
        
        template<typename J,typename H>
        void
        GradOp<J,H>::reset(){
            
            typedef typename H::value_type value_type;
            
            std::vector<value_type> empty;
            res_.swap(empty);
        }
        
        template<typename J,typename H>
        void
        GradOp<J,H>::zero(){
            typedef typename H::value_type value_type;
            std::for_each(res_.begin(),
                          res_.end(),
                           [](value_type& val){
                               val=utilities::scalar_value_traits<value_type>::zero();});
        }
        
        template<typename J,typename H>
        void
        GradOp<J,H>::copy_to(GradOp<J,H>& other)const{
            
            other.error_func_ = error_func_;
            other.h_ = h_;
        }

        template<typename J,typename H>
        void
        GradOp<J,H>::operator ()(const utilities::range1d<size_type>& range){
#ifdef PARML_DEBUG
            using utilities::ExeLogicError;
            std::string msg = "Null Error Function Pointer";
            Assert(error_func_ != nullptr,ExeLogicError(msg));
            
            msg = "Null Hypothesis Function Pointer";
            Assert(h_ != nullptr,ExeLogicError(msg));
#endif

            typedef typename H::value_type value_type;
            if(res_.empty())
                res_.resize(h_->n_coeffs(),utilities::scalar_value_traits<value_type>::zero());
            else
                zero();

            error_func_->range_gradients(res_,range,*h_);
        }

        template<typename J,typename H>
        void
        GradOp<J,H>::join(const typename GradOp<J,H>::result_type& other){

#ifdef PARML_DEBUG
                using utilities::ExeLogicError;
                const std::string msg=" Invalid size for joining: "+std::to_string(other.size())
                              +" should be "+std::to_string(res_.size());
                Assert(res_.size() == other.size(),ExeLogicError(msg));
#endif 

            for(size_type i=0; i<res_.size(); ++i){

                res_[i] += other[i];
            }    
        }
    }//par_batch_gd_detail
        
}//parml



#endif	/* BATCH_GRADIENT_DESCENT_DETAIL_H */

