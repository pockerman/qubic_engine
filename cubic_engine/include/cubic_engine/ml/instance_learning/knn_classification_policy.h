/* 
 * File:   knn_classification_policy.h
 * Author: david
 *
 * Created on October 17, 2016, 8:24 PM
 */

#ifndef KNN_CLASSIFICATION_POLICY_H
#define	KNN_CLASSIFICATION_POLICY_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/knn_policy_base.h"
#include <boost/scoped_array.hpp>

namespace cengine
{
    
   
/**
 * @brief Class that performs classification using
 * K-nearest neighbors algorithm
 */
class KnnClassificationPolicy: public knn_policy_base<false>
{
    
public:
    
    /**
     * @brief Expose the type of the base
     */
    typedef knn_policy_base<false> base_type;
    
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
     //typedef base_type::comm_chunk comm_chunk;
    
    /**
     * @brief Constructor
     */
    KnnClassificationPolicy(uint_t k);
    
    /**
     * @brief Return the result of the classification 
     */
    return_type get_result()const;
    
    /**
     * @brief Return the result of regression based on the given
     * class index,class counter pairs
     */
    //return_type get_result(const boost::scoped_array<comm_chunk>& results,size_type nelems)const;
    
};
 
}



#endif	/* KNN_CLASSIFICATION_POLICY_H */

