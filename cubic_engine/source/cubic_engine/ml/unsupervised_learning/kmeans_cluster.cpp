#include "parml/unsupervised_learning/kmeans_cluster.h"
#include "parml/datasets/armadillo_matrix_dataset.h"

namespace parml
{
    
    
template<typename DP_TYPE> 
KMeansCluster<DP_TYPE>::KMeansCluster()
:
id(ParML::iuint()),
center(),
points()
{}
    
template<typename DP_TYPE> 
KMeansCluster<DP_TYPE>::KMeansCluster(size_type idx)
:
id(idx),
center(),
points()
{}
    
    
template<typename DP_TYPE> 
KMeansCluster<DP_TYPE>::KMeansCluster(size_type idx,const DP_TYPE& c)
:
id(idx),
center(c),
points()
{}
    
template<typename DP_TYPE>
bool 
KMeansCluster<DP_TYPE>::add_to_cluster(size_type pid){
    
    /*typedef std::set<size_type>::iterator iterator;
    std::pair<iterator,bool> inserted = points.insert(pid);
    return inserted.second; */ 
    
    
    typedef std::vector<size_type>::iterator iterator;
        
    iterator it = std::find_if(points.begin(),points.end(),
                               [pid](size_type p)->bool{return (p == pid);});
        
    if(it !=points.end()){
       return false;
    }
        
    points.push_back(pid);
            
    return true;
}

template<typename DP_TYPE>
void 
KMeansCluster<DP_TYPE>::add_to_cluster(const std::vector<size_type>&/*std::set<size_type>&*/ pts){
    
    if(pts.empty() || pts.size() == static_cast<size_type>(0))
                return;

    size_type current_size = points.size();
    size_type pts_size = pts.size();

    points.reserve(current_size + pts_size);
    
    for(size_type p=0; p<pts.size(); ++p){
       add_to_cluster(pts[p]); 
    }

    //points.insert(pts.begin(),pts.end());
}

template<typename DP_TYPE>
bool 
KMeansCluster<DP_TYPE>::remove_from_cluster(size_type pid){
    
    /*size_type erased = points.erase(pid);
    
    if(erased != 0)
        return true;
    
    return false;*/
    
    typedef std::vector<size_type>::iterator iterator;

    iterator it = std::find_if(points.begin(),points.end(),
                                    [=](size_type p)->bool{return p == pid;});

    if(it !=points.end()){

        points.erase(std::remove_if(points.begin(),points.end(),
                                    [=](size_type p)->bool{return (p == pid);}));

        return true;  
    }
                                    
    return false;
}


//explicit instantiations
template class KMeansCluster<ArmaVecDataSet<size_type> >;
template class KMeansCluster<ArmaVecDataSet<real_type> >;
template class KMeansCluster<ArmaVecDataSet<float> >;
template class KMeansCluster<ArmaVecDataSet<int> >;    
    
}
