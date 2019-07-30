#include "parml/instance_learning/knn_classification_policy.h"


namespace parml
{
    
   knn_classify_policy::knn_classify_policy(size_type k)
   :
   knn_policy_base<false>(k)
   {}
    
   knn_classify_policy::return_type 
   knn_classify_policy::get_result()const{
       
       if(this->data_handler_.majority_vote.empty()){
           ParML::iuint();   
       }
       
       auto voteitr  = this->data_handler_.majority_vote.begin();
       auto voteitre = this->data_handler_.majority_vote.end();
       
       //the class index
       size_type rslt = voteitr->first;
       
       //the class counter
       size_type ctr  = voteitr->second;
    
       while(voteitr != voteitre){
        
                size_type rslttmp = voteitr->first;
                size_type ctrtmp  = voteitr->second;
        
                if(ctrtmp > ctr){
                    ctr = ctrtmp;
                    rslt = rslttmp;
                }

                voteitr++;
        }

        return rslt;      
   }
   
   
   knn_classify_policy::return_type 
   knn_classify_policy::get_result(const boost::scoped_array<knn_classify_policy::comm_chunk>& results,
                                   size_type nelems)const{
       
       
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
   }
}
