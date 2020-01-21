#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_info.h"

namespace cengine
{
    
   
    
    KMeansInfo::KMeansInfo()
    :
    AlgInfo(),
    clusters(),
    n_clustering_points(0)
    {}
    
    
    std::ostream& 
    KMeansInfo::print(std::ostream& out)const{ 
        out<<"================================"<<std::endl;
        this->AlgInfo::print(out);
        out<<"# of clustering points: "<<n_clustering_points<<std::endl;
        out<<"# clusters: "<<clusters.size()<<std::endl;
        
        uint_t ntotal_clustered_points = 0;
        typedef std::vector<std::pair<uint_t, uint_t> >::const_iterator const_iterator;
        const_iterator cb = clusters.begin();
        const_iterator ce = clusters.end();
        
        while(cb!=ce){
            ntotal_clustered_points += (*cb).second;
            out<<"Cluster: "<<(*cb).first<<" n points: "<<(*cb).second<<std::endl;
            cb++;
        }
        
        out<<"# of total clustered points: "<<ntotal_clustered_points<<std::endl;
        out<<"================================"<<std::endl;
        return out;
    }   
}//parml
