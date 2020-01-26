#include "cubic_engine/ml/instance_learning/utils/knn_policy_base.h"
#include "kernel/base/kernel_consts.h"
#include <limits>

namespace cengine
{
    
knn_policy_base_data_handler<true>::return_type knn_policy_base_data_handler<true>::invalid_result_value = 
                std::numeric_limits<knn_policy_base_data_handler<true>::return_type>::max();   
     
knn_policy_base_data_handler<true>::knn_policy_base_data_handler(uint_t k_)
:
k(k_),
k_distances(),
majority_vote()
{}


/*void
knn_policy_base_data_handler<true>::get_top_k_results(boost::scoped_array<knn_policy_base_data_handler<true>::comm_chunk>& arr )const{
    
#ifdef PARML_DEBUG
    std::string msg = "In knn_policy_base_data_handler<true>::get_top_k_results(). k_distances is empty";
    using utilities::ExeLogicError;
    Assert(k_distances.empty()==false,ExeLogicError(msg));
    msg = "In knn_policy_base_data_handler<true>::get_top_k_results(). Incompatible sizes: majority_vote size: "+
            std::to_string(majority_vote.size())+" not equal to k_distances size: "+
            std::to_string(k_distances.size());
    Assert(k_distances.size()==majority_vote.size(),ExeLogicError(msg));
#endif    

    typedef knn_policy_base_data_handler<true>::comm_chunk comm_chunk;
    arr.reset(new comm_chunk[k_distances.size()]);
    
    for(size_type r=0; r<k_distances.size(); ++r){
        
        arr[r] = parframepp::serializable_pair<real_type,real_type>(k_distances[r].second,majority_vote[r]);
    }   
}*/

knn_policy_base_data_handler<false>::return_type knn_policy_base_data_handler<false>::invalid_result_value = kernel::KernelConsts::invalid_size_type();

knn_policy_base_data_handler<false>::knn_policy_base_data_handler(uint_t k_)
:
k(k_),
k_distances(),
majority_vote()
{}


/*void
knn_policy_base_data_handler<false>::get_top_k_results(boost::scoped_array<knn_policy_base_data_handler<false>::comm_chunk>& arr)const{
    
    
    
#ifdef PARML_DEBUG
    std::string msg = "In knn_policy_base_data_handler<false>::get_top_k_results(). majority_vote is empty";
    using utilities::ExeLogicError;
    Assert(majority_vote.empty()==false,ExeLogicError(msg));
#endif

    typedef knn_policy_base_data_handler<false>::comm_chunk comm_chunk;
    arr.reset(new comm_chunk[majority_vote.size()]);

    auto start_ = majority_vote.begin();
    auto end_ = majority_vote.end();
    size_type r =0;
    while(start_ != end_){
        
        arr[r++] = parframepp::serializable_pair<size_type,size_type>(start_->first,start_->second);
        start_++;
    }
   
}*/

template<bool is_regressor>
knn_policy_base<is_regressor>::knn_policy_base(uint_t k)
:
data_handler_(k)
{}

template<bool is_regressor>
void 
knn_policy_base<is_regressor>::resume(uint_t k){
       
#ifdef PARML_DEBUG
    
    using utilities::ExeLogicError;
    const std::string msg = "In knn_class_policy::resume(). Invalid number of neighbors: "+std::to_string(k);
    Assert(((k != ParML::iuint()) || (k != 0)),ExeLogicError(msg));
#endif
       
       data_handler_.k = k;
       data_handler_.majority_vote.clear();
       data_handler_.k_distances.clear();
   }

//explicit instantiations
template class knn_policy_base<true>;
template class knn_policy_base<false>;
    
}
