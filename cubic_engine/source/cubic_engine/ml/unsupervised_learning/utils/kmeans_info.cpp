#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_info.h"

namespace cengine
{
    
   
    
    KMeansInfo::KMeansInfo()
    :
    AlgInfo()
    {}
    
    
    std::ostream& 
    KMeansInfo::print(std::ostream& out)const{ 

        this->AlgInfo::print(out);
        return out;
    }   
}//parml
