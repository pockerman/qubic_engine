#include "cubic_engine/ml/instance_learning/details/knn_classification_policy.h"
#include "kernel/base/kernel_consts.h"


namespace cengine{
namespace ml {


    
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
}
}
