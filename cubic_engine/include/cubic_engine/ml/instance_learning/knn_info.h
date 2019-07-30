/* 
 * File:   knn_info.h
 * Author: david
 *
 * Created on October 19, 2016, 1:56 PM
 */

#ifndef KNN_INFO_H
#define	KNN_INFO_H

#include "parml/base/parml.h"
#include "parml/instance_learning/knn_type.h"

#include <chrono>
#include <ostream>

namespace parml
{
    
    
/**
 * @brief Small struct that holds information about
 * the performance of the knn algorithm
 */
struct KnnInfo
{
    
    
 
    /**
     * @brief How many threads the algorithm used
     */
    size_type nthreads;
    
    /**
     * @brief How many processors the algorithm used
     */
    size_type nprocs; 
    
    /**
     * @brief The type of the knn algorithm
     */
    knn_type::Type type;
    
    /**
     * @brief The total time duration of the algorithm
     */
    std::chrono::duration<real_type> runtime;
    
    /**
     * @brief how many points it predicted
     */
    size_type n_pts_predicted;
    
    /**
     * @brief The number of neighbors used
     */
    size_type n_neighbors;
    
    
    /**
     * @brief Constructor
     */
    KnnInfo();
    
    
    /**
     * @brief Print the information about the performance of
     * the algorithm on the given stream
     */
    std::ostream& print(std::ostream& out)const; 
        
};  


inline
std::ostream& operator<<(std::ostream& out,const KnnInfo& info ){
    
    return info.print(out);
}




}



#endif	/* KNN_INFO_H */

