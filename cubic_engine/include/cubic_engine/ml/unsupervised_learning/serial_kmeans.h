
#ifndef SERIAL_KMEANS_H
#define	SERIAL_KMEANS_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_info.h"
#include "cubic_engine/ml/unsupervised_learning/utils/kmeans_control.h"
#include "cubic_engine/ml/unsupervised_learning/utils/cluster.h"

#include "kernel/parallel/utilities/result_holder.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/utilities/csv_file_writer.h"

#include <exception>
#include <iostream>
#include <chrono>
#include <tuple>

namespace cengine
{
    

/// \brief Implementation of KMeans
template<typename ClusterType>
class KMeans
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
    typedef std::vector<cluster_t> result_t;
    
    /// \brief Constructor
    KMeans(const KMeansControl& cntrl);
        
    /// \brief Cluster the given data set
    template<typename DataIn, typename Similarity,typename Initializer>
    output_t cluster(const DataIn& data, const Similarity& similarity, const Initializer& init);

    /// \brief Return the clusters container
    result_t& get_clusters(){return clusters_;}

    /// \brief Return the clusters container
    const result_t& get_clusters()const{return clusters_;}

    /// \brief Save the clustering into a csv file
    template<typename DataSetType>
    void save(const std::string& file_name, const DataSetType& data_in)const;

private:

    /// \brief The algorithm control
    control_t control_;

    /// \brief The clusters
    std::vector<cluster_t> clusters_;

    /// \brief Detect convergence
    template<typename Similarity>
    std::tuple<bool, real_t> detect_convergence_(const Similarity& sim,
                                                    const std::vector<point_t>& old_centers)const;
    /// \brief Actually cluster the given point
    template<typename Similarity>
    void cluster_point_(const point_t& point, uint_t pid, const Similarity& sim );

    /// \brief Check if an empty cluster exists
    bool check_empty_clusters_()const;

    /// \brief Calculate the new centroids of the clusters
    template<typename DataSetType>
    void calculate_new_centroids_(const DataSetType& dataset);
};

template<typename ClusterType>
KMeans<ClusterType>::KMeans(const KMeansControl& cntrl)
    :
   control_(cntrl),
   clusters_()
{}


template<typename ClusterType>
template<typename DataIn, typename Similarity, typename Initializer>
typename KMeans<ClusterType>::output_t
KMeans<ClusterType>::cluster(const DataIn& data, const Similarity& similarity, const Initializer& init){


    typedef typename KMeans<ClusterType>::cluster_t cluster_t;
    typedef typename KMeans<ClusterType>::output_t output_t;

    output_t info;
    auto k = control_.k;
    auto rows = data.rows();


    // more clusters than data does not make
    // sense
    if(k > rows){
        throw std::logic_error("Number of clusters cannot be larger than number of rows");
    }

 //start timing 
 std::chrono::time_point<std::chrono::system_clock> start, end;
 start = std::chrono::system_clock::now();
 
 random_restart:
 
    //the old centroids
    std::vector<typename ClusterType::point_t> centroids;

    {
        std::vector<cluster_t> empty;
        clusters_.swap(empty);
    }

    //initialize the clusters
    init(data, k, centroids);

    if(centroids.size() != k){
        throw std::logic_error("Incorrect centroid initialization: "+
                               std::to_string(centroids.size()) +
                               " not equal to: "+
                               std::to_string(k));
    }

    clusters_.resize(k);
    for(uint_t c=0; c<clusters_.size(); ++c){
        clusters_[c].centroid = centroids[c];
    }

    bool converged = false;
    real_t residual = std::numeric_limits<real_t>::max();

    while (control_.continue_iterations()) {

        if(control_.show_iterations()){

            std::cout<<"\tK-means iteration: "<<control_.get_current_iteration()<<std::endl;
        }

        for(uint_t r=0; r<data.rows(); ++r){

            // try to cluster the point
            cluster_point_(kernel::get_row(data, r), r, similarity);
        }

        // check if we have empty clusters
        auto empty_cluster = check_empty_clusters_();

        if(empty_cluster){

            if ( control_.show_iterations()){
                std::cout<<"\t\tEmpty cluster detected..."<<std::endl;
            }

            if(!control_.continue_on_empty_cluster && control_.random_restart_on_empty_cluster){

                if(control_.show_iterations()){
                    std::cout<<"\t\tRestarting..."<<std::endl;
                }

                goto random_restart;
            }
            else if(!control_.continue_on_empty_cluster){
                break;
            }
            else if(control_.show_iterations()){
                std::cout<<"\t\tContinue with empty cluster detected..."<<std::endl;
            }
        }

        // clalculate new centroids
        calculate_new_centroids_(data);

        // check if we converged
        auto [converged, residual] = detect_convergence_(similarity, centroids);

        control_.update_residual(residual);

        if(converged){
            break;
        }

    }
 
    auto state = control_.get_state();
    end = std::chrono::system_clock::now();

    info.runtime = end-start;
    info.nprocs = 1;
    info.nthreads = 1;
    info.converged = state.converged;
    info.residual = state.residual;
    info.tolerance = state.tolerance;
    info.niterations = state.num_iterations;
    return info;
}

template<typename ClusterType>
template<typename Similarity>
std::tuple<bool, real_t>
KMeans<ClusterType>::detect_convergence_(const Similarity& sim,
                                         const std::vector<typename ClusterType::point_t>& old_centers)const{

    if(clusters_.size() != old_centers.size()){
        const std::string msg = "Current number of clusters: "+std::to_string(clusters_.size())+
                                " not equal of old number of clusters: "+std::to_string(old_centers.size());
        throw std::logic_error(msg);
    }

    bool converged = true;
    auto dis = std::numeric_limits<real_t>::max();

    for(auto c=0; c<clusters_.size(); ++c){

        dis = sim(clusters_[c].centroid, old_centers[c]);

        if(dis > Similarity::tolerance()){
            converged = false;
            break; //we don't have to continue calculating distances
        }
    }

    return {converged, dis};
}

template<typename ClusterType>
template<typename Similarity>
void
KMeans<ClusterType>::cluster_point_(const point_t& point, uint_t pid, const Similarity& sim ){

    typedef typename Similarity::value_t value_t;

    value_t current_dis = std::numeric_limits<value_t>::max();
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

template<typename ClusterType>
bool
KMeans<ClusterType>::check_empty_clusters_()const{
    return false;
}

template<typename ClusterType>
template<typename DataSetType>
void
KMeans<ClusterType>::calculate_new_centroids_(const DataSetType& dataset){

    for(uint_t c=0; c<clusters_.size(); ++c){
        clusters_[c].recalculate_centroid(dataset);
    }
}

template<typename ClusterType>
template<typename DataSetType>
void
KMeans<ClusterType>::save(const std::string& file_name, const DataSetType& data)const{


    kernel::CSVWriter writer(file_name, kernel::CSVWriter::default_delimiter(), true);

    std::vector<std::string> names(data.columns() + 1);

    names[0] = "ClusterId";

    for(uint_t i=1; i<names.size(); ++i){
        names[i] = "X-"+std::to_string(i);
    }

    //write the names
    writer.write_row(names);

    std::vector<real_t> row(names.size());

    for(uint_t c=0; c<clusters_.size(); ++c){

        auto& cluster = clusters_[c];

        for(uint_t p=0; p<cluster.points.size(); ++p){

            uint_t pidx = cluster.points[p];
            auto point = kernel::get_row(data, pidx);
            row[0] = cluster.id;

            for(uint_t r=0; r<point.size(); ++r){
                row[r+1] = point[r];
            }

           writer.write_row(row);
        }
    }

    //close the file
    writer.close();
}

    
}



#endif	/* SERIAL_KMEANS_H */

