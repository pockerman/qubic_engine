#include "parml/unsupervised_learning/k_means.h"

namespace parml
{
    
   
    
    KMeansInfo::KMeansInfo()
    :
    AlgInfo(),
    clusters()
    {}
    
    
    std::ostream& 
    KMeansInfo::print(std::ostream& out)const{ 
        this->AlgInfo::print(out);
        out<<"Number of clusters: "<<clusters.size()<<std::endl;
        typedef std::vector<std::pair<size_type,size_type> >::const_iterator const_iterator;
        const_iterator cb = clusters.begin();
        const_iterator ce = clusters.end();
        
        while(cb!=ce){
            
            out<<"Cluster: "<<(*cb).first<<" n points: "<<(*cb).second<<std::endl;
            cb++;
        }
        return out;
    }   
}//parml
