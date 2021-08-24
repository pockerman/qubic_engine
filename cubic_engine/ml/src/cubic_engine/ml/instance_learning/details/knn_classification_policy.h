#ifndef KNN_CLASSIFICATION_POLICY_H
#define	KNN_CLASSIFICATION_POLICY_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/details/knn_policy_base.h"

namespace cengine{
namespace ml {

///
/// \brief Class that performs classification using
/// K-nearest neighbors algorithm
///
class KnnClassificationPolicy: public ml::details::knn_policy_base<false>
{
    
public:
    
    ///
    /// \brief Expose the type of the base
    ///
    typedef ml::details::knn_policy_base<false> base_t;
    
    ///
    /// \brief Expose the return type of the policy
    ///
    typedef base_t::return_type return_t;
    
    ///
    /// \brief The type of the pair used for storing row-distance values
    ///
    typedef base_t::Pair Pair;
    
    ///
    /// \brief The type of the container that holds the distances computed
    ///
    typedef base_t::distances_container_type distances_container_type;

    ///
    /// \brief Expose the type of the container that holds the majority vote
    ///
    typedef base_t::majority_vote_container_type majority_vote_container_type;
     
    ///
    /// \brief Constructor
    ///
    explicit KnnClassificationPolicy(uint_t k);

    ///
    /// \brief Return the result of the classification
    ///
    return_t get_result()const;
       
};

 
}
}


#endif	/* KNN_CLASSIFICATION_POLICY_H */

