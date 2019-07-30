#include "parml/unsupervised_learning/kmeans_detail.h"

namespace parml
{
    
    
    namespace kmeans_detail{
        
#ifdef PARML_HAS_MPI
        void resize_receive_cluster_container(std::vector<std::vector<ClusterPoint> >& points,
                                              const std::vector<utilities::range1d<size_type> > partitions){
            
            if(ParML::pid() == parframepp::rt_base<mpi_par>::master_process_id()){
        
                    //all but the master process send to the master
                    points.resize(ParML::n_processors()-1);
        
                    for(size_type p=0; p<ParML::n_processors(); ++p){
            
                        if(p != parframepp::rt_base<mpi_par>::master_process_id()){
#ifdef PARML_DEBUG
    using utilities::ExeInvalidIndex;
    const std::string msg="The Index: "+std::to_string(p)+" Not In [0,"+std::to_string(partitions.size())+")";
    Assert(p<partitions.size(),ExeInvalidIndex(msg));
#endif
                            size_type how_many = partitions[p].end()-partitions[p].begin();
                            points[p-1].resize(how_many);
                        }   
                    }    
            }  
        }
#endif
        
    }//detail
    
    
}
