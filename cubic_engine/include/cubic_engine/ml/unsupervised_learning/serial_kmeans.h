/* 
 * File:   serial_kmeans.h
 * Author: david
 *
 * Created on October 24, 2016, 9:01 AM
 */

#ifndef SERIAL_KMEANS_H
#define	SERIAL_KMEANS_H

#include "parml/unsupervised_learning/kmeans_base.h"
#include "parframepp/base/platform_enum_type.h"

#include <chrono>

namespace parml
{
    
//forward declarations
template<parframepp::platform_type::Type t,typename DataPoint> class KMeans;
template<typename T> class ArmaVecDataSet;

    
/**
 * @brief Serial implementation of KMeans
 */ 
template<typename DataPoint>
class KMeans<parframepp::platform_type::Type::SERIAL,DataPoint>: public KMeansBase<DataPoint>
{
    
public:
    
    /**
      * @brief Constructor
      */
    KMeans(const KMeansControl& cntrl);
        
        
    /**
     * @brief Cluster the given data set
     */
    template<typename DataIn,typename Similarity,typename Initializer>
    void cluster(const DataIn& data,const Similarity& similarity,const Initializer& init);
        
};

//template and inline methods

template<typename DataPoint>
template<typename DataIn,typename Similarity,typename Initializer>
void 
KMeans<parframepp::platform_type::Type::SERIAL,DataPoint>::cluster(const DataIn& data,
                                                                   const Similarity& similarity,const Initializer& init){

    size_type k = this->cntrl_.k;
    size_type rows = data.n_rows();
    size_type max_itrs = this->cntrl_.max_n_iterations;

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



    
}



#endif	/* SERIAL_KMEANS_H */

