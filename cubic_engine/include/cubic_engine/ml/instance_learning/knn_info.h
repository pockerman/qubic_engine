/* 
 * File:   knn_info.h
 * Author: david
 *
 * Created on October 19, 2016, 1:56 PM
 */

#ifndef KNN_INFO_H
#define	KNN_INFO_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <chrono>
#include <ostream>

namespace cengine
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
    uint_t nthreads;
    
    /**
     * @brief How many processors the algorithm used
     */
    uint_t nprocs;
    
    /**
     * @brief The total time duration of the algorithm
     */
    std::chrono::duration<real_t> runtime;
    
    /**
     * @brief how many points it predicted
     */
    uint_t n_pts_predicted;
    
    /**
     * @brief The number of neighbors used
     */
    uint_t n_neighbors;
    
    
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

