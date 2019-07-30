/* 
 * File:   knn_regression_policy.h
 * Author: david
 *
 * Created on October 19, 2016, 2:28 PM
 */

#ifndef KNN_AVERAGE_REGRESSION_POLICY_H
#define	KNN_AVERAGE_REGRESSION_POLICY_H


#include "parml/instance_learning/knn_policy_base.h"
#include <boost/scoped_array.hpp>

namespace parml
{
    
    
/**
 * @brief Policy class for regression with K-nn algorithm.
 * This policy uses the average of the values the k nearest neighbors
 * found to predict the value of the given input point
 */
class knn_avg_regression_policy:public knn_policy_base<true>
{
    
    
public:
    
    /**
     * @brief Expose the type of the base
     */
    typedef knn_policy_base<true> base_type;
    
    /**
     * @brief Expose the return type of the policy
     */
    typedef base_type::return_type return_type;
    
    /**
     * @brief The type of the pair used for storing row-distance values
     */
    typedef base_type::Pair Pair;
    
    /**
     * @brief The type of the container that holds the distances computed
     */
     typedef base_type::distances_container_type distances_container_type;
     
     /**
      * @brief Expose the type of the container that holds the majority vote
      */
     typedef base_type::majority_vote_container_type majority_vote_container_type;
     
     /**
     * @brief Expose the type of the object needed for communication when using MPI
     */
     typedef base_type::comm_chunk comm_chunk;
     
     
     /**
      * @brief Constructor 
      */
     knn_avg_regression_policy(size_type k);
     

    /**
     * @brief Return the result of regression
     */
    return_type get_result()const;
    
    /**
     * @brief Return the result of regression based on the given
     * distance,value pairs
     */
    return_type get_result(const boost::scoped_array<comm_chunk>& results,size_type nelems)const;
        
};
        
}



#endif	/* KNN_REGRESSION_POLICY_H */

