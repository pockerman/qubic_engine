/* 
 * File:   kmeans_base.h
 * Author: david
 *
 * Created on July 28, 2016, 1:51 PM
 */

#ifndef KMEANS_BASE_H
#define	KMEANS_BASE_H
#include "parml/base/parml.h"
#include "parml/base/parml_runtime.h"
#include "parml/base/algorithm_control.h"
#include "parml/unsupervised_learning/kmeans_detail.h"

#include "utilities/file_writer_csv.h"
#include "parml/base/parml_config.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#endif

#include <ostream>
#include <string>

namespace parml
{
    
/**
 * @brief A struct that describes various configuration
 * parameters for running the kmeans algorithm
 */
struct KMeansControl:public AlgControl
{
    /**
     * @brief Number of clusters
     */
    size_type k;
    
    /**
     * @brief Flag indicating whether a random restart should
     * be performed when an empty cluster is located
     */
    bool random_restart_on_empty_cluster;
    
    /**
     * @brief Flag indicating whether the algorithm should
     * continue its execution when an empty cluster is detected 
     */
    bool continue_on_empty_cluster;
    
    /**
     * @brief The number of features
     */
    size_type n_features;
    
    /**
     * @brief The number of data points
     */
    size_type n_training_examples;
    
    /**
     * @brief Constructor
     */
    KMeansControl(size_type k_,size_type itrs=1000)
            :
            AlgControl(itrs,ParML::tolerance()),
            k(k_),
            random_restart_on_empty_cluster(true),
            continue_on_empty_cluster(false),
            n_features(ParML::iuint()),
            n_training_examples(ParML::iuint())
    {}
    
     /**
     * @brief print the control data to the given stream
     */
    virtual std::ostream& print(std::ostream& out)const;
    
};

inline
std::ostream& operator<<(std::ostream& out,const KMeansControl& control ){
    return control.print(out);
}
 
/**
 * @brief Common functionality for K-means algorithm
 * 
 */
class kmeans_base
{
    
public:
    
    /**
     * @brief Constructor
     */
    kmeans_base(const KMeansControl& data)
    :
    data_(data)
    {}
    
    /**
      * @brief Save the clustering into a csv file
      */
    template<typename DataInput,typename CentroidType> 
    void save(const std::string& file_name,const DataInput& data_in,
              const std::vector<kmeans_detail::Cluster<CentroidType> >& clusters)const;
    
    
    /**
      * @brief Returns true if and only if the distance between the newly calculated
      * centroid and the old centroid for every cluster is less than tol
      */
    template<typename Similarity,typename CentroidType>
    bool detect_convergence(const Similarity& sim,
                            const std::vector<kmeans_detail::Cluster<CentroidType> >& clusters,
                            const std::vector<CentroidType>& old_centers);
    
    /**
     * @brief Initialize the centroids and the clusters arrays
     * using the Initializer object
     */
    template<typename Initializer,typename DataInput,typename CentroidType>
    void initialize_clusters(const Initializer& init,
                             const DataInput& data_in,
                             std::vector<CentroidType>& centroids,
                             std::vector<kmeans_detail::Cluster<CentroidType> >& clusters);
    
    
    /**
     * @brief Return true if and only if any of the given clusters is empty
     */
    template<typename CentroidType>
    bool detect_empty_cluster(const std::vector<kmeans_detail::Cluster<CentroidType> >& clusters);
    
protected:
    
    /**
     * @brief The configuration parameters for running the kmeans algorithm
     */
    KMeansControl data_;
};


template<typename DataInput,typename CentroidType> 
void 
kmeans_base::save(const std::string& file_name,const DataInput& data_in,
                  const std::vector<kmeans_detail::Cluster<CentroidType> >& clusters)const{
    
    //if(ParML::pid() == parframepp::RTBase<mpi_par>::master_process_id()){
    
        const size_type ncols = data_.n_features;
    
#ifdef PARML_DEBUG
        using utilities::ExeLogicError;
        const std::string msg="Invalid number of columns. Columns:  "+
                            std::to_string(ncols)+" not equal to: "+
                            std::to_string(data_adaptor::second_size(data_in));
        Assert(ncols == data_adaptor::second_size(data_in),ExeLogicError(msg));
#endif 
    
        utilities::file_writer_csv writer(file_name);
        writer.open();
  
        std::vector<std::string> names(ncols+1);
    
        names[0] = "ClusterId";
    
        for(size_type n=1; n<names.size(); ++n){
            names[n] = "X"+std::to_string(n);
        }
    
        //write the names
        writer.write_header(names);
    
        std::vector<real_type> row(names.size());
        for(size_type c=0; c<clusters.size(); ++c){

            row[0] = static_cast<real_type>(clusters[c].id);

            const std::vector<kmeans_detail::ClusterPoint>& points = clusters[c].points;

            for(size_type p=0; p<points.size(); ++p){

               const CentroidType point = data_adaptor::data_point(data_in,points[p].pid);

               for(size_type col=1; col<names.size(); ++col){

                   row[col] = point[col-1];
               } 

               writer.write_row(row); 
            }      
        }
    
        //close the file
        writer.close(); 
    
    //}
}


template<typename Similarity,typename CentroidType>
bool 
kmeans_base::detect_convergence(const Similarity& sim,
                                const std::vector<kmeans_detail::Cluster<CentroidType> >& clusters,
                                const std::vector<CentroidType>& old_centers){
       
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
        
        real_type dis = sim(clusters[c].location,old_centers[c]);
        
        if(dis > data_.tolerance){
            converged = false;
            break; //we don't have to continue calculating distances
        }
    }
    
    return converged;   
}

template<typename Initializer,typename DataInput,typename CentroidType>
void 
kmeans_base::initialize_clusters(const Initializer& init,
                                 const DataInput& data_in,
                                 std::vector<CentroidType>& centroids,
                                 std::vector<kmeans_detail::Cluster<CentroidType> >& clusters){
    
    const size_type nrows = data_.n_training_examples;
   
#ifdef PARML_DEBUG
    using utilities::ExeLogicError;
    const std::string msg="Invalid number of rows. Rows:  "+
                            std::to_string(nrows)+" not equal to: "+
                            std::to_string(data_adaptor::size(data_in));
    Assert(nrows == data_adaptor::size(data_in),ExeLogicError(msg));
#endif
    
    std::pair<size_type,size_type> range(0,nrows-1);
    
    //initialize the centroids
    init.initialize(data_in,range,data_.k,centroids);
    
    //assign the initial location of the cluster centroid
    //and the cluster id
    for(size_type c=0; c<centroids.size(); ++c){
        
        clusters[c].id = c;
        clusters[c].assign_location(centroids[c]);
    }   
}


template<typename CentroidType>
bool 
kmeans_base::detect_empty_cluster(const std::vector<kmeans_detail::Cluster<CentroidType> >& clusters){
    
    
    //check for empty cluster
    for(size_type c=0; c<clusters.size(); ++c){
            
        if(clusters[c].points.empty() || clusters[c].points.size()==0){
                
            return true;
        }
    }
    
    return false;
}

    
    
}



#endif	/* KMEANS_BASE_H */

