#ifndef KNN_AVERAGE_REGRESSION_POLICY_H
#define	KNN_AVERAGE_REGRESSION_POLICY_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/utils/knn_policy_base.h"
#include <boost/scoped_array.hpp>

namespace cengine
{
    
    
/**
 * @brief Policy class for regression with K-nn algorithm.
 * This policy uses the average of the values the k nearest neighbors
 * found to predict the value of the given input point
 */
class KnnAvgRegressionPolicy:public knn_policy_base<true>
{
    
    
public:
    
    /**
     * @brief Expose the type of the base
     */
    typedef knn_policy_base<true> base_t;
    
    /**
     * @brief Expose the return type of the policy
     */
    typedef base_t::return_type return_t;
    
    /**
     * @brief The type of the pair used for storing row-distance values
     */
    typedef base_t::Pair Pair;
    
    /**
     * @brief The type of the container that holds the distances computed
     */
     typedef base_t::distances_container_type distances_container_type;
     
     /**
      * @brief Expose the type of the container that holds the majority vote
      */
     typedef base_t::majority_vote_container_type majority_vote_container_type;
     
     /**
      * @brief Constructor 
      */
     KnnAvgRegressionPolicy(uint_t k);
     
    /**
     * @brief Return the result of regression
     */
    return_type get_result()const;
           
};
        
}



#endif	/* KNN_REGRESSION_POLICY_H */

