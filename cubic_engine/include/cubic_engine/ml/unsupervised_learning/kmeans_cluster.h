/* 
 * File:   kmeans_cluster.h
 * Author: david
 *
 * Created on October 24, 2016, 9:30 AM
 */

#ifndef KMEANS_CLUSTER_H
#define	KMEANS_CLUSTER_H

#include "parml/base/parml.h"
#include "parml/datasets/armadillo_vec_dataset.h"

#include <set>


namespace parml
{

//forward declarations
template<typename T> class ArmaMatDataSet;



    
/**
 * @brief Small struct that represents a cluster
 * for the K-means algorithm
 */
template<typename DP_TYPE>
struct KMeansCluster
{
  
    /*
     * @brief The id of the cluster
     */
    size_type id;
    
    /**
      * @brief The location of the cluster centroid
      */
    DP_TYPE center;
    
    
    /**
     * @brief Ids of the points assign to the cluster 
     */
    //std::set<size_type> points;
    
    std::vector<size_type> points;
    
    /**
     * @brief Expose the type of the container that holds the clustered points
     */
    //typedef std::set<size_type> points_container_type;
    typedef std::vector<size_type> points_container_type;
    
    /**
     * @brief Iterator over the points in this cluster
     */
    typedef points_container_type::iterator iterator;
    
    /**
     * @brief Iterator over the points in this cluster
     */
    typedef points_container_type::const_iterator const_iterator;
    
    /**
     * @brief Constructor 
     */
    KMeansCluster();
    
    /**
     * @brief Constructor 
     */
    KMeansCluster(size_type id);
    
    
    /**
     * @brief Constructor 
     */
    KMeansCluster(size_type id,const DP_TYPE& c);
    
    
    /**
      * @brief Adds to this cluster the point with id pid. Returns
      * true if and only if the given point already exists in this
      * cluster which in this case does nothing.
      */
    bool add_to_cluster(size_type pid);
    
    
    /**
     * @brief Add the following list of points into the cluster
     */
    //void add_to_cluster(const std::set<size_type>& pts);
    void add_to_cluster(const std::vector<size_type>& pts);
    
    
    /**
      * @brief Remove from this cluster the point with the given id.
      * Returns true if and only if the point is found. Otherwise, it returns
      * false and does not remove anything.
      */
    bool remove_from_cluster(size_type pid);
    
    
    /**
      * @brief Assign the location of the cluster
      */
    void assign_center(const DP_TYPE& p){center = p;}
    
    
    /**
      * @brief Recalculates the new centroid for the cluster based on
      * the data points that have been assigned to the cluster
      */
    template<typename DataSet>
    void recalculate_centroid(const DataSet& input);
    
    
    /**
     * @brief Return an iterator at the beginning of the clustered
     * points list
     */
    iterator begin(){return points.begin();}
    
     /**
     * @brief Return an iterator at one past the end position of the clustered
     * points list
     */
    iterator end(){return points.end();}
    
    
    /**
     * @brief Return an iterator at the beginning of the clustered
     * points list
     */
    const_iterator begin()const{return points.begin();}
    
     /**
     * @brief Return an iterator at one past the end position of the clustered
     * points list
     */
    const_iterator end()const{return points.end();}
    
    
    
    
    /**
     *@brief Assign the given DP_TYPE data point
     * in one of the clusters using the given Similarity metric
     */
    template<typename Similarity>
    static void cluster(const DP_TYPE& dp,size_type r,const Similarity& sim,
                        std::vector<KMeansCluster<DP_TYPE> >& clusters);
    
     /**
     * @brief Return true if and only if any of the clusters is empty alongside with the id
      * of the empty cluster. 
     */
    static std::pair<bool,size_type> detect_empty_cluster(const std::vector<KMeansCluster<DP_TYPE> >& clusters );
    
    
    /**
     * @brief Given the Similarity metric and the new and old centers
     * check if the algorithm has converged. 
     */
    template<typename Similarity>
    static bool detect_convergence(const Similarity& sim,const std::vector<DP_TYPE>& centers,
                                   const std::vector<KMeansCluster<DP_TYPE> >& clusters,
                                   real_type tol);
    
    
    
};

//template and inline methods

template<typename DP_TYPE>
template<typename DataSet>
void 
KMeansCluster<DP_TYPE>::recalculate_centroid(const DataSet& input){
    
     //nothing to do if this cluster has no
     //points
     if(points.empty()){
        return;
     }    

    //the new centroid will be the average
    //of the points in the cluster
    DP_TYPE avg(center.size());
    
    auto itrb = points.begin();
    auto itre = points.end();
    
    while(itrb!=itre){
        
        avg += input.row(*itrb++);
    }
            
    avg /= static_cast<typename DP_TYPE::value_type>(points.size());
    center = avg;       
}

template<typename DP_TYPE>
template<typename Similarity>
void 
KMeansCluster<DP_TYPE>::cluster(const DP_TYPE& point,size_type pid,const Similarity& sim,
                                std::vector<KMeansCluster<DP_TYPE> >& clusters){
    
    typedef typename DP_TYPE::value_type value_type;
    value_type current_dis = std::numeric_limits<value_type>::max();
    size_type cluster_id = ParML::iuint();

    for(size_type c=0; c<clusters.size(); ++c){

#ifdef PARML_DEBUG
    using utilities::InvalidRange;
    const std::string msg = "In L2Distance::operator(). Incompatible Range: "+
                            std::to_string(point.size())+", "+std::to_string(clusters[c].center.size());
    //std::cout<<"v1 size: "<<point.size()<<" v2 size: "<<clusters[c].center.size()<<std::endl;
    Assert(point.size() == clusters[c].center.size(),InvalidRange(msg));
#endif
       value_type dis = sim(point,clusters[c].center);  

       if(dis -current_dis < ParML::tolerance()){

           current_dis = dis;
           cluster_id = c;
       }
    }
    
#ifdef PARML_DEBUG
            
//make sure we have a valid cluster id
using  utilities::ExeLogicError;
const std::string msg = "In KMeansCluster<DP_TYPE>::cluster. Cluster id: "+
                         std::to_string(cluster_id)+" not in [0,"+
                         std::to_string(clusters.size())+")";
Assert(cluster_id < clusters.size(),ExeLogicError(msg));           
#endif

    //add to the new cluster 
    //and remove the id from the others. Since each point is assigned
    //to exactly one cluster we don't have to remove from the rest
    //of the clusters if the point was found.
            
    bool found  = clusters[cluster_id].add_to_cluster(pid);
            
    if(!found){
                
        for(size_type c=0; c<clusters.size(); ++c){
                    
            if(c!= cluster_id && !found){
                found = clusters[c].remove_from_cluster(pid);
            }
        }  
    }       
}
            
    
    


template<typename DP_TYPE>
std::pair<bool,size_type> 
KMeansCluster<DP_TYPE>::detect_empty_cluster(const std::vector<KMeansCluster<DP_TYPE> >& clusters ){
    
    
    //check for empty cluster
    for(size_type c=0; c<clusters.size(); ++c){
            
        if(clusters[c].points.empty() || clusters[c].points.size()== 0){
                
            return std::make_pair(true,c);
        }
    }
    
    return std::make_pair(false,ParML::iuint());      
}

template<typename DP_TYPE>
template<typename Similarity>
bool 
KMeansCluster<DP_TYPE>::detect_convergence(const Similarity& sim,
                                           const std::vector<DP_TYPE>& old_centers,
                                           const std::vector<KMeansCluster<DP_TYPE> >& clusters,real_type tol){
    
    
#ifdef PARML_DEBUG
    size_type nclusters = clusters.size();
    size_type nold_centers = old_centers.size();
    using utilities::ExeSizeMismatch;
    const std::string msg = "Current number of clusters: "+std::to_string(nclusters)+
                            " not equal of old number of clusters: "+std::to_string(nold_centers);
    Assert(nclusters == nold_centers,ExeSizeMismatch(msg));
#endif
    
    bool converged = true;
    
    for(size_type c=0; c<clusters.size(); ++c){
        
        real_type dis = sim(clusters[c].center,old_centers[c]);
        
        if(dis > tol){
            converged = false;
            break; //we don't have to continue calculating distances
        }
    }
    
    return converged;   
}
    
    
}



#endif	/* KMEANS_CLUSTER_H */

