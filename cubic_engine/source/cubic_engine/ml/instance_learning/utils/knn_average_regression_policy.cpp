#include "cubic_engine/ml/instance_learning/utils/knn_average_regression_policy.h"
#include <algorithm>


namespace cengine
{
    
 
KnnAvgRegressionPolicy::KnnAvgRegressionPolicy(uint_t k)
:
knn_policy_base<true>(k)
{}


KnnAvgRegressionPolicy::return_type
KnnAvgRegressionPolicy::get_result()const{
    
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg="In knn_avg_regression_policy::get_result(). majority_vote size: "+
                          std::to_string(this->data_handler_.majority_vote.size())+
                          " not equal to k: "+std::to_string(this->data_handler_.k);
    Assert(this->data_handler_.majority_vote.size()== this->data_handler_.k,ExeLogicError(msg));
#endif 


    
    
    //std::cout<<"In knn_avg_regression_policy::get_result(). majority_vote size: "<<this->data_handler_.majority_vote.size()<<std::endl;
    
    /*real_t avg = 0.0;
    avg = std::accumulate(this->data_handler_.majority_vote.begin(),
                    this->data_handler_.majority_vote.end(),avg);*/
    return 0.0; //avg/this->data_handler_.k;
}

/*knn_avg_regression_policy::return_type
knn_avg_regression_policy::get_result(const boost::scoped_array<knn_avg_regression_policy::comm_chunk>& results,
                                      size_type nelems)const{
    
    
    typedef knn_avg_regression_policy::comm_chunk comm_chunk;
    
    std::vector<comm_chunk> distances;
    distances.reserve(nelems);
    
    for(size_type e=0; e<nelems; ++e){
        
       distances.push_back(results[e]); 
    }
    
    //sort the computed pairs based on the distances
    std::sort(distances.begin(),distances.end(),
             [](const comm_chunk& p1,const comm_chunk& p2){
               return p1.first < p2.first;});
               
    real_type rslt = 0.0;
    
    for(size_type e=0; e<this->data_handler_.k; ++e){
        
        rslt += distances[e].second;
    }
    
    return rslt/this->data_handler_.k;   
}*/
    
    
    
}
