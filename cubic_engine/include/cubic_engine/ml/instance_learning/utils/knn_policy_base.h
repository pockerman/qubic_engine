/* 
 * File:   knn_policy_base.h
 * Author: david
 *
 * Created on October 19, 2016, 2:30 PM
 */

#ifndef KNN_POLICY_BASE_H
#define	KNN_POLICY_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/utilities/map_utilities.h"
#include "kernel/maths/matrix_utilities.h"
#include <boost/scoped_array.hpp>

#include <vector>
#include <utility>
#include <map>
#include <iostream>

namespace cengine
{
    
    
/**
 * @brief Small struct that holds the right
 * data structures for classification and regression policies
 */
template<bool is_regressor> struct knn_policy_base_data_handler;


/**
 * @brief Explicit specializations. Data handler
 * for regression policy
 */
template<>
struct knn_policy_base_data_handler<true>
{
    
   /**
     * @brief Expose the return type of the policy
     */
    typedef real_t return_type;
    
    /**
     * @brief The type of the container that holds
     * the majority vote
     */
    typedef std::vector<real_t> majority_vote_container_type;
    
    /**
     * @brief The type of the pair used for storing row-distance values
     */
    typedef std::pair<uint_t, real_t> Pair;
    

    /**
     * @brief The value that indicates that the policy has returned an invalid result.
     * By default this is numeric_limits<return_type>::max()
     */
    static return_type invalid_result_value;
    
    /**
     * @brief Return the value that indicates an invalid result
     */
    static return_type invalid_result(){return invalid_result_value;}
    
    /**
     * @brief Set the value for the policy that indicates an invalid result
     */
    static void set_invalid_result_value(return_type val){invalid_result_value = val;}
    
    
    /**
     * @brief Number of neighbors
     */
    uint_t k;
    
    
    /**
      * @brief The top k distances computed by the regressor
      * coupled with row id that the distance occurred
      */
     
    std::vector<Pair> k_distances;
     
     
     /**
      * @brief The vector that holds the values of the labels
      * vector for the k-top neighbors
      */
    std::vector<real_t> majority_vote;
     
     /**
      * @brief Constructor
      */
    knn_policy_base_data_handler(uint_t k_);
     
     
     /**
       * @brief Given the labels vector and having computed the
       * the top k-distances store the values that correspond to these
       * top distances 
       */
    template<typename DataVec>
    void fillin_majority_vote(const DataVec& labels);
              
};


template<typename DataVec>
void 
knn_policy_base_data_handler<true>::fillin_majority_vote(const DataVec& labels){
     
    if( k <= k_distances.size()){
        throw std::logic_error("Incompatible number of neighbors: "+
                               std::to_string(k)+
                               " and k_distances size: "+
                               std::to_string(k_distances.size()));
    }


    //we loop ove all the k-distances as we want
    //the average
    for(uint_t d=0; d<k; ++d){
        
        uint_t row_idx = k_distances[d].first;
        real_t value = labels[row_idx];
        majority_vote.push_back(value);
    }
}


/**
 * @brief Explicit specializations. Data handler
 * for classification policy
 */
template<>
struct knn_policy_base_data_handler<false>
{
    
   /**
     * @brief Expose the return type of the policy
     */
    typedef uint_t return_type;
    
    /**
     * @brief The type of the container that holds
     * the majority vote
     */
    typedef std::map<uint_t, uint_t> majority_vote_container_type;
    
    /**
     * @brief The type of the pair used for storing row-distance values
     */
    typedef std::pair<uint_t, real_t> Pair;
    

     /**
     * @brief The value that indicates that the policy has returned an invalid result.
     * By default this is numeric_limits<return_type>::max()
     */
    static return_type invalid_result_value;
    
    /**
     * @brief Return the value that indicates an invalid result
     */
    static return_type invalid_result(){return invalid_result_value;}
    
    /**
     * @brief Set the value for the policy that indicates an invalid result
     */
    static void set_invalid_result_value(return_type val){invalid_result_value = val;}
    
    /**
     * @brief Number of neighbors
     */
    uint_t k;
    
    
    /**
      * @brief The top k distances computed by the classifier
      * coupled with row id that the distance occurred
      */
    std::vector<Pair> k_distances;
     
     
     /**
      * @brief The vector that holds the values of the labels
      * vector for the k-top neighbors
      */
    std::map<uint_t, uint_t> majority_vote;
    
     
     /**
      * @brief Constructor
      */
    knn_policy_base_data_handler(uint_t k_);
     
     
     /**
       * @brief Given the labels vector and having computed the
       * the top k-distances store the values that correspond to these
       * top distances 
       */
    template<typename DataVec>
    void fillin_majority_vote(const DataVec& labels);
            
};



template<typename DataVec>
void 
knn_policy_base_data_handler<false>::fillin_majority_vote(const DataVec& labels){
    

    if(k < k_distances.size()){
        throw std::logic_error("Number of neighbors: "+
                               std::to_string(k)+
                               " not compatible with: "+
                               std::to_string(k_distances.size()));
    }
    
   typedef std::map<uint_t, uint_t>::iterator iterator;
    
   for(uint_t i=0; i<k; ++i){
        
       uint_t idx = k_distances[i].first;
       uint_t cls = labels[idx];

       kernel::add_or_update_map(majority_vote, cls, static_cast<uint_t>(1),
                                [](iterator itr){itr->second += 1;});
   }
}
    

/**
 * @brief Base class that holds common
 * functionality for classification and regression
 * policies for K-nn algorithm
 */
template<bool is_regressor>
class knn_policy_base
{
    
public:
    
    /**
     * @brief Expose the return type of the policy
     */
    typedef typename knn_policy_base_data_handler<is_regressor>::return_type return_type;
    
    /**
     * @brief Expose the majority vote container type of the policy
     */
    typedef typename knn_policy_base_data_handler<is_regressor>::majority_vote_container_type majority_vote_container_type;
    
    /**
     * @brief Expose the type of the object that holds (row index,distance) pairs
     */
    typedef typename knn_policy_base_data_handler<is_regressor>::Pair Pair;
    
    
    /**
     * @brief Expose the type of the container that holds the computed distances
     */
    typedef std::vector<Pair> distances_container_type;
    

    /**
     * @brief Return the value that indicates an invalid result
     */
    static return_type invalid_result(){return knn_policy_base_data_handler<is_regressor>::invalid_result_value;}
    
    /**
     * @brief Set the value for the policy that indicates an invalid result
     */
    static void set_invalid_result_value(return_type val){knn_policy_base_data_handler<is_regressor>::set_invalid_result_value(val);}
    
    /**
     * @brief Constructor
     */
    knn_policy_base(uint_t k);
    
    
    /**
     * @brief Return the number of neighbors used for the classification
     */
    uint_t n_neighbors()const{return data_handler_.k;}
    
    
    /**
     * @brief Predict the result for the given data point. 
     * 
     * When working serially the range includes all the rows of the DataInput matrix.
     * 
     * When working on multi-core machines we assume that the entire data set can 
     * be held in RAM the range parameter holds the rows indices the thread
     * is working on
     * 
     * When working with MPI then \f$data\f$ may be all in the working process
     * or a chunk of it. In the former case we simply iterate over the row 
     * indices supplied in the given \f$range\f$. If \f$data\f$ is simply
     * a chunk of the whole data then indices in \f$range\f$ should start from zero. 
     * In any case this function expects that the indices have been set up correctly
     * by the client code.
     */
    template<typename DataMat, typename LabelType, typename DataVec,typename Similarity>
    void operator()(const DataMat& data, const LabelType& labels, const DataVec& input,
                    const Similarity& sim, const kernel::range1d<uint_t>& range);
    
    
    /**
     * @brief Given the labels vector compute the k most relevant
     * indexes for the classification
     */
    template<typename DataVec>
    void fillin_majority_vote(const DataVec& labels);
    
    /**
     * @brief Given the labels vector and the distances
     * compute the k most relevant indexes for classification. It overrides
     * any computed distances by the object and the majority vote map.
     * It is assumed that the distances vector is not too large
     * and hence this operation can be done serially. Moreover, the distances
     * are assumed to be sorted
     */
    template<typename DataVec>
    void fillin_majority_vote(const DataVec& labels, 
                              std::vector<std::pair<uint_t,real_t> >&& distances);
    
    
    /**
     * @brief Return the distances computed 
     */
    distances_container_type& get_distances(){return data_handler_.k_distances;}
    
     /**
     * @brief Return the distances computed 
     */
    const distances_container_type& get_distances()const{return data_handler_.k_distances;}
    
    
    /**
     * @brief Resume the object to a state just like when calling the constructor
     */
    void resume(uint_t k);
    
    /**
     * @brief Resume the object to a state just like when calling the constructor but
     * keep the number of neighbors
     */
    void resume(){resume(data_handler_.k);}
    
    
protected:
    
    
    /**
     * @brief The data handler
     */
    knn_policy_base_data_handler<is_regressor> data_handler_;
    
};

template<bool is_regressor>
template<typename DataMat, typename LabelType, typename DataVec,typename Similarity>
void 
knn_policy_base<is_regressor>::operator()(const DataMat& data,  const LabelType& labels, const DataVec& point,
                                          const Similarity& sim, const kernel::range1d<uint_t>& range){
    
    typedef typename knn_policy_base<is_regressor>::Pair Pair;
     
    //we will calculating as many distances as
    //the size of the range
    std::vector<Pair> distances;
    distances.reserve(range.size());
    
    for(uint_t r=range.begin(); r<range.end(); ++r){
        
        //access the r-th row of the matrix
        const DataVec& data_point = kernel::get_row(data, r);
        
        //compute the distance between the data point and the 
        //input point
        real_t dis = sim(data_point, point);

        //compute the similarity and append it to distances
        distances.push_back(std::make_pair(r,dis));  
    }
    
    //sort the computed pairs based on the distances
    std::sort(distances.begin(),distances.end(),
             [](const Pair& p1,const Pair& p2){return p1.second < p2.second;});
    
    {
        //empty what has been already computed 
        std::vector<Pair> empt;
        data_handler_.k_distances.swap(empt);
        data_handler_.majority_vote.clear();
    }
               
    data_handler_.k_distances.reserve(data_handler_.k);
    

    for(uint_t i=0; i<data_handler_.k; ++i){
       data_handler_.k_distances.push_back(distances[i]);
    }   
    
    //fill in the majority vote
    fillin_majority_vote(labels);
}

template<bool is_regressor>
template<typename DataVec>
void 
knn_policy_base<is_regressor>::fillin_majority_vote(const DataVec& labels){
    
    data_handler_.fillin_majority_vote(labels);
}

template<bool is_regressor>
template<typename DataVec>
void 
knn_policy_base<is_regressor>::fillin_majority_vote(const DataVec& labels, 
                                                    std::vector<std::pair<uint_t,real_t> >&& distances){
    
    data_handler_.k_distances = std::move(distances);
    distances.clear();
    fillin_majority_vote(labels);   
}

    
}



#endif	/* KNN_POLICY_BASE_H */

