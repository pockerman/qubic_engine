#include "cubic_engine/ml/instance_learning/knn_classification_policy.h"
#include "kernel/base/kernel_consts.h"


namespace cengine
{
    
   KnnClassificationPolicy::KnnClassificationPolicy(uint_t k)
   :
   knn_policy_base<false>(k)
   {}
    
   KnnClassificationPolicy::return_type
   KnnClassificationPolicy::get_result()const{
       
       if(this->data_handler_.majority_vote.empty()){
           return kernel::KernelConsts::invalid_size_type();
       }
       
       auto voteitr  = this->data_handler_.majority_vote.begin();
       auto voteitre = this->data_handler_.majority_vote.end();
       
       //the class index
       uint_t rslt = voteitr->first;
       
       //the class counter
       uint_t ctr  = voteitr->second;
    
       while(voteitr != voteitre){
        
         uint_t rslttmp = voteitr->first;
         uint_t ctrtmp  = voteitr->second;
        
         if(ctrtmp > ctr){
             ctr = ctrtmp;
             rslt = rslttmp;
         }

         voteitr++;
       }

        return rslt;      
   }
   
   
   /*KnnClassificationPolicy::return_type
   KnnClassificationPolicy::get_result(const boost::scoped_array<knn_classify_policy::comm_chunk>& results,
                                         uint_t nelems)const{
       
       
        size_type rslt = ParML::iuint();
    
        //the class index
        rslt = results[0].first;
       
        //the class counter
        size_type ctr  = results[0].second;
    
        for(size_type e=0; e<nelems; ++e){
        
            size_type rslttmp = results[e].first;
            size_type ctrtmp  = results[e].second;
        
            if(ctrtmp > ctr){
                        ctr = ctrtmp;
                        rslt = rslttmp;
            }
        }
    
        return rslt;    
   }*/
}
