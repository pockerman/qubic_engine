/* 
 * File:   kmeans_base.h
 * Author: david
 *
 * Created on October 25, 2016, 2:18 PM
 */

#ifndef KMEANS_BASE_H
#define	KMEANS_BASE_H

#include "parml/base/parml_config.h"
#include "parml/unsupervised_learning/kmeans_control.h"
#include "parml/unsupervised_learning/kmeans_info.h"
#include "parml/unsupervised_learning/kmeans_cluster.h"

#include "parframepp/base/platform_enum_type.h"

#include <utility>
#include <vector>

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

namespace parml
{
    
//forward declarations
template<typename T> class ArmaVecDataSet;
 
template<typename DataPoint>
class KMeansBase
{
    
public:
    
    
    /**
     * @brief An enumeration holding the types of action to perform
     */
    enum class ActionType{RESTART,BREAK,CONVERGED,CONTINUE,INVALID_TYPE};
    
    /**
     * @brief Expose the type of the data point
     */
    typedef DataPoint data_point_type;
    
    
    /**
     * @brief Constructor
     */
    KMeansBase(const KMeansControl& cntrl);
    
    
    /**
     * @brief Return a reference to the KMeansInfo 
     */
    KMeansInfo& get_info(){return info_;}
    
    /**
     * @brief Return the clusters container
     */
    std::vector<KMeansCluster<DataPoint> >& get_clusters(){return clusters_;}
    
    /**
     * @brif Return the clusters container
     */
    const std::vector<KMeansCluster<DataPoint> >& get_clusters()const{return clusters_;}
    
    
    
protected:
    
    /**
     * @brief The object that controls the algorithm
     */
    const KMeansControl& cntrl_;
    
    /**
     * @brief The object that holds information for the algorithm
     */
    KMeansInfo info_;
    
    /**
     * @brief The clusters
     */
    std::vector<KMeansCluster<DataPoint> > clusters_;
    

    
};


namespace kmeans_detail
{
    
#ifdef PARML_DEBUG
    
    void assert_comon(const std::string& func_msg,size_type nrows,size_type k,size_type max_itrs);
#endif
    
    
    /**
     * @brief Initialize the centers when the data
     * exist on all processors
     */
    struct DataOnProcInitializer
    {
    
    
        template<typename DataSet,typename Initializer>
        static void initialize(const DataSet& set,const Initializer& init,size_type k,
                               std::vector<KMeansCluster<typename DataSet::Row> >& clusters,
                               std::vector<typename DataSet::Row >& centroids);
    };
    
    

    template<typename DataSet,typename Initializer>
    void 
    DataOnProcInitializer::initialize(const DataSet& set,
                                      const Initializer& init,size_type k,
                                      std::vector<KMeansCluster<typename DataSet::Row> >& clusters,
                                      std::vector<typename DataSet::Row >& centroids){
        centroids.clear();
        clusters.clear();
        const size_type nrows = set.n_rows(); 
        std::pair<size_type,size_type> range(0,nrows-1);
    
        //initialize the clusters
        init.initialize(set,range,k,centroids);
               
#ifdef PARML_DEBUG
        using utilities::ExeLogicError;
        const std::string msg="In DataOnProcInitializer::initialize. Number of centroids: "+
        std::to_string(centroids.size())+" not equal to number of clusters k: "+std::to_string(k);
        Assert(centroids.size() == k, ExeLogicError(msg));
#endif
        
         clusters.resize(k);
    
        //assign the initial location of the cluster centroid
        //and the cluster id
        for(size_type c=0; c<centroids.size(); ++c){
        
            clusters[c].id = c;
            clusters[c].assign_center(centroids[c]);
        }   
    
    }
    
    
    /**
     * @brief General class that controls the iterations
     */
    struct ExitCondition
    {

        /**
         * @brief Return true if and only if the exit condition is satisfied
         */
        bool check();

        /**
         * @brief The current interation index
         */
        size_type current_itr;

        /**
         * @brief How many iterations to do
         */
        size_type n_max_itrs;

        /**
         * @brief Constructor 
         */
        ExitCondition(size_type max_itrs)
                :
                current_itr(0),
                n_max_itrs(max_itrs)
        {}   
    };


    inline
    bool
    ExitCondition::check(){

        if(current_itr < n_max_itrs){
            current_itr++;
            return false;
        }

        return true;
    }
    
    /**
      * @brief Class that wraps together the actions to be performed in a given
      * iteration
      */
    template<parframepp::platform_type::Type t> struct Iteration;


    template<> 
    struct Iteration<parframepp::platform_type::Type::SERIAL>
    {


       template<typename DataIn,typename Similarity>
       typename KMeansBase<typename DataIn::Row>::ActionType 
       operator()(const DataIn& data,
                  const Similarity& sim,
                  std::vector<KMeansCluster<typename DataIn::Row> >& clusters,
                  std::vector<typename DataIn::Row>& centroids); 

       /**
        * @brief The control object
        */
       const KMeansControl& cntrl;



       /**
        * @brief Constructor
        */
       Iteration(const KMeansControl& cntrl_ )
               :
               cntrl(cntrl_)
       {}
    };

    template<typename DataIn,typename Similarity>
    typename KMeansBase<typename DataIn::Row>::ActionType
    Iteration<parframepp::platform_type::Type::SERIAL>::operator()(const DataIn& data,
                                                                   const Similarity& sim,
                                                                   std::vector<KMeansCluster<typename DataIn::Row> >& clusters,
                                                                   std::vector<typename DataIn::Row>& centroids){
        typedef typename DataIn::Row DataPoint;

        size_type nrows = data.n_rows();

        //loop over the data points
        for(size_type r=0; r<nrows;  ++r){

            DataPoint p = data.row(r); 

            //cluster the data point
            KMeansCluster<DataPoint>::cluster(p,r,sim,clusters);
        }

        //check for empty cluster
        std::pair<bool,size_type> empty_cluster_found = KMeansCluster<DataPoint>::detect_empty_cluster(clusters); 

        if(empty_cluster_found.first && cntrl.random_restart_on_empty_cluster){

           if(cntrl.show_iterations){
              std::cout<<"\tAn empty cluster was found  restarting..."<<std::endl;
         }

          return KMeansBase<DataPoint>::ActionType::RESTART;
        }
        else if(empty_cluster_found.first && !cntrl.continue_on_empty_cluster){

            if(cntrl.show_iterations){
               std::cout<<"\tAn empty cluster was found  breaking..."<<std::endl;
            }
            return KMeansBase<DataPoint>::ActionType::BREAK;
        }

        //recalculate the centroids for each cluster
        for(size_type c=0; c<clusters.size(); ++c){

           clusters[c].recalculate_centroid(data);
        }

        bool converged = KMeansCluster<DataPoint>::detect_convergence(sim,centroids,clusters,cntrl.tolerance);

        if(converged){


           if(cntrl.show_iterations){
             std::cout<<"\tKMeans converged with tolerance: "<<cntrl.tolerance<<std::endl;
           }

          return KMeansBase<DataPoint>::ActionType::CONVERGED;   
        }

        //update the old centers
        for(size_type c=0; c<centroids.size(); ++c){
            centroids[c] = clusters[c].center; 
        }
        
        return KMeansBase<DataPoint>::ActionType::CONTINUE;
    }

}//kmeans_detail
}



#endif	/* KMEANS_BASE_H */

