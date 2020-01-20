
#ifndef SERIAL_KMEANS_H
#define	SERIAL_KMEANS_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/kmeans_info.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"
#include "cubic_engine/ml/unsupervised_learning/utils/cluster.h"

#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/base/kernel_consts.h"

#include <exception>
#include <chrono>

namespace cengine
{
    

/// \brief Implementation of KMeans
template<typename ClusterType>
class KMeans: public KMeansBase<DataPoint>
{
    
public:

    /// \brief The output type returned upon completion
    /// of the algorithm
    typedef KMeansInfo output_t;

    /// \brief The input to the algorithm
    typedef KMeansControl control_t;

    /// \brief The cluster type used
    typedef ClusterType cluster_t;

    /// \brief The centroid type
    typedef typename ClusterType::point_t point_t;

    /// \brief The result after computing
    typedef kernel::ResultHolder<std::vector<cluster_t>> result_t;
    
    /// \brief Constructor
    KMeans(const KMeansControl& cntrl);
        
    /// \brief Cluster the given data set
    template<typename DataIn, typename Similarity,typename Initializer>
    output_t cluster(const DataIn& data,const Similarity& similarity,const Initializer& init);

    /// \brief Save the clustering into a csv file
    template<typename DataInput,typename CentroidType>
    void save(const std::string& file_name, const DataInput& data_in)const;

private:

    /// \brief The algorithm control
    control_t control_;

    /// \brief The clusters
    std::vector<cluster_t> clusters_;


    /// \brief Detect convergence
    template<typename Similarity>
    bool detect_convergence_(const Similarity& sim,
                             const std::vector<point_t>& old_centers)const;

    /// \brief Actually cluster the given point
    template<typename Similarity>
    void cluster_point_(const point_t& point, uint_t pid, const Similarity& sim );
};

//template and inline methods

template<typename DataPoint>
template<typename DataIn,typename Similarity,typename Initializer>
void 
KMeans<DataPoint>::cluster(const DataIn& data, const Similarity& similarity,const Initializer& init){

    auto k = control_.k;
    auto rows = data.n_rows();
    auto max_itrs = control_.max_n_iterations;

#ifdef PARML_DEBUG
    const std::string func_msg = "In KMeans<parframepp::platform_type::Type::SERIAL,DataPoint>::cluster(). ";
    kmeans_detail::assert_comon(func_msg,rows,k,max_itrs);
#endif
    
 //start timing 
 std::chrono::time_point<std::chrono::system_clock> start, end;
 start = std::chrono::system_clock::now();
 
 random_restart:
 
 
    //the old centroids
    std::vector<DataPoint> centroids;
 
    //initialize the clusters
    kmeans_detail::DataOnProcInitializer::initialize(data,init,this->cntrl_.k,this->clusters_,centroids);
 
    //the exit condition
    kmeans_detail::ExitCondition exit_cond(this->cntrl_.max_n_iterations);
    
    //the object that handles the iteration
    kmeans_detail::Iteration<parframepp::platform_type::Type::SERIAL> iteration(this->cntrl_);
    
    //what the algorithm should do at each iteration
    typename KMeansBase<DataPoint>::ActionType action = KMeansBase<DataPoint>::ActionType::INVALID_TYPE;
    
    while(exit_cond.check() == false){
        
        if(this->cntrl_.show_iterations){
            
            std::cout<<"\tK-means iteration: "<<exit_cond.current_itr<<std::endl;
        }
        
        action = iteration(data,similarity,this->clusters_,centroids);
        
        if(action == KMeansBase<DataPoint>::ActionType::RESTART)
            goto random_restart;
        else if(action == KMeansBase<DataPoint>::ActionType::BREAK)
            break;
        else if(action == KMeansBase<DataPoint>::ActionType::CONVERGED){
            this->info_.converged = true;
            break;   
        }
    }
    
    if(action == KMeansBase<DataPoint>::ActionType::CONVERGED){
        
        this->info_.clusters.resize(this->clusters_.size());
        for(size_type c=0; c<this->clusters_.size(); ++c){
            this->info_.clusters[c]=std::make_pair(c,this->clusters_[c].points.size());   
        }
    }
    
    end = std::chrono::system_clock::now();
    this->info_.runtime = end-start;    
}

template<typename ClusterType>
template<typename Similarity>
bool
KMeans<ClusterType>::detect_convergence_(const Similarity& sim,
                                         const std::vector<typename ClusterType::point_t>& old_centers)const{

    if(clusters_.size() != old_centers.size()){
        const std::string msg = "Current number of clusters: "+std::to_string(clusters_.size())+
                                " not equal of old number of clusters: "+std::to_string(old_centers.size());
        throw std::logic_error(msg);
    }

    bool converged = true;

    for(auto c=0; c<clusters_.size(); ++c){

        auto dis = sim(clusters_[c].centroid, old_centers[c]);

        if(dis > Similarity::tolerance){
            converged = false;
            break; //we don't have to continue calculating distances
        }
    }

    return converged;
}

template<typename ClusterType>
template<typename Similarity>
void
KMeans<ClusterType>::cluster_point_(const point_t& point, uint_t pid, const Similarity& sim ){

    typedef typename Similarity::value_type value_type;

    value_type current_dis = std::numeric_limits<value_type>::max();
    auto cluster_id = kernel::KernelConsts::invalid_size_type();

    for(auto c=0; c<clusters_.size(); ++c){

        auto dis = sim(point, clusters_[c].centroid);

        if(dis - current_dis < Similarity::tolerance()){

            current_dis = dis;
            cluster_id = c;
        }
    }

    if(cluster_id < clusters_.size()){

        //make sure we have a valid cluster id
        const std::string msg = "Cluster id: "+
                            std::to_string(cluster_id)+" not in [0,"+
                            std::to_string(clusters_.size())+")";
        throw std::logic_error(msg);
    }


    // add to the new cluster
    // and remove the id from the others. Since each point is assigned
    // to exactly one cluster we don't have to remove from the rest
    // of the clusters if the point was found.

    bool found  = clusters_[cluster_id].add_to_cluster(pid);

    if(!found){

        for(auto c=0; c<clusters_.size(); ++c){

            if(c!= cluster_id && !found){
                found = clusters_[c].remove_from_cluster(pid);
            }
        }
    }
}



    
}



#endif	/* SERIAL_KMEANS_H */

