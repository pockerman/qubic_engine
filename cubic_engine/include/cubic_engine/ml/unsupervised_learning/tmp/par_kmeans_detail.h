/* 
 * File:   kmeans_tasks.h
 * Author: david
 *
 * Created on July 28, 2016, 1:28 PM
 */

#ifndef PAR_KMEANS_DETAIL_H
#define	PAR_KMEANS_DETAIL_H

#include "parml/base/parml.h"
#include "parml/unsupervised_learning/kmeans_detail.h"
#include "utilities/range1d.h"
#include "parframepp/parallel/task_base.h"

#include <vector>

namespace parml
{
    namespace par_kmeans_detail
    {
        
        template<typename DataInput,typename CentroidType,typename Similarity>
        struct Task: public parframepp::TaskBase 
        {
            
            public:
                
                Task(const utilities::range1d<size_type>& range,
                     const DataInput& datain,
                     const Similarity& s)
                :
                TaskBase(),
                range_(range),
                data_in_(&datain),
                clusters_(),
                sim(s)
                {}
                
                /**
                 * @brief get the locally computed result
                 * @return 
                 */
                std::vector<kmeans_detail::Cluster<CentroidType> >& get_local_clusters()
                {return clusters_;}
                 
            protected:
                
               
                virtual parframepp::TaskBase* run()final; 
                
                /**
                 * @brief the range of indices over which the task runs
                 */
                utilities::range1d<size_type> range_;
                
                /**
                 * @brief Pointer to the input data matrix
                 */
                const DataInput* data_in_;
                
                
                /**
                 * @brief The clusters that the task manages
                 */
                std::vector<kmeans_detail::Cluster<CentroidType> > clusters_;
                
                /**
                 * @brief The similarity function the task uses to compute
                 * the distances from the clusters centroids
                 */
                const Similarity& sim;                   
        };
        
        template<typename DataInput,typename CentroidType,typename Similarity>
        parframepp::TaskBase*
        Task<DataInput,CentroidType,Similarity>::run(){
    
            //loop over the data points assigned to this task
            for(size_type r=range_.begin(); r<range_.end(); ++r){
            
                CentroidType p = data_adaptor::data_point(*data_in_,r);
            
                //cluster the data point
                kmeans_detail::cluster(p,r,sim,clusters_);
            }
    
            return nullptr;
        }
        
        template<typename CentroidType>
        struct RemoveTask: public parframepp::TaskBase
        {
            
            public:
            
                RemoveTask(std::vector<kmeans_detail::Cluster<CentroidType> >& clusters)
                        :
                parframepp::TaskBase(),
                clusters_(&clusters),
                points_(nullptr),
                remove_cluster_id_(ParML::iuint())
                {}
                
                /**
                 * @brief Set the points to be removed
                 */
                void set_points_to_remove(const std::vector<kmeans_detail::ClusterPoint>& pts)
                {points_ = &pts;}
                
                void set_remove_from_cluster_id(size_type cid){remove_cluster_id_ = cid;}
                
            protected:
            
                /**
                 * @brief Run the task
                 */
                virtual parframepp::TaskBase* run()final; 
                
                
                /**
                 * @brief The cluster pool from which to remove points
                 */
                std::vector<kmeans_detail::Cluster<CentroidType> >* clusters_;
                
                
                /**
                 * @brief The points to remove
                 */
                const std::vector<kmeans_detail::ClusterPoint>* points_;
                
                
                /**
                 * @brief The ids of the clusters from which we remove data points
                 */
                size_type remove_cluster_id_;
        };
        
        template<typename CentroidType>
        parframepp::TaskBase*
        RemoveTask<CentroidType>::run(){
    
#ifdef PARML_DEBUG
        using utilities::ExeNullPointer;
        Assert(points_!=nullptr,ExeNullPointer());
#endif
    
            if(points_->empty())
                return nullptr;

            for(size_type c=0; c<clusters_->size(); ++c){

                if(c!=remove_cluster_id_)
                    (*clusters_)[c].remove_from_cluster(*points_); 
            }
       
            return nullptr;
        }
          
    }//par_kmeans_detail
}



#endif	/* KMEANS_TASKS_H */

