/* 
 * File:   knn_policy_base.h
 * Author: david
 *
 * Created on October 19, 2016, 2:30 PM
 */

#ifndef KNN_POLICY_BASE_H
#define	KNN_POLICY_BASE_H

#include "parml/base/parml_config.h"
#include "parml/base/parml.h"
#include "utilities/range1d.h"
#include "utilities/data_input.h"
#include "utilities/map_utilities.h"
#include "parframepp/parallel_utils/serializable_pair.h"

#ifdef PARML_DEBUG
#include "utilities/exceptions.h"
#include <string>
#endif

#include <boost/scoped_array.hpp>

#include <vector>
#include <utility>
#include <map>

namespace parml
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
    typedef real_type return_type;
    
    /**
     * @brief The type of the container that holds
     * the majority vote
     */
    typedef std::vector<real_type> majority_vote_container_type;
    
    /**
     * @brief The type of the pair used for storing row-distance values
     */
    typedef std::pair<size_type,real_type> Pair;
    
    /**
     * @brief The type of the object to communicate when working with MPI.
     * The first type is the distance and the second is the value
     */
    typedef parframepp::serializable_pair<real_type,real_type> comm_chunk;
    
    
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
    size_type k;
    
    
    /**
      * @brief The top k distances computed by the regressor
      * coupled with row id that the distance occurred
      */
     
    std::vector<Pair> k_distances;
     
     
     /**
      * @brief The vector that holds the values of the labels
      * vector for the k-top neighbors
      */
    std::vector<real_type> majority_vote;
     
     /**
      * @brief Constructor
      */
    knn_policy_base_data_handler(size_type k_);
     
     
     /**
       * @brief Given the labels vector and having computed the
       * the top k-distances store the values that correspond to these
       * top distances 
       */
    template<typename DataVec>
    void fillin_majority_vote(const DataVec& labels);
    
    
    /**
     * @brief Return the top-k results.  Return
     * a dynamic array of pairs (distance,value). This
     * function is used in the implementation of MPI K-nn algorithm.
     * In general, we don't expect that there will be a large number
     * of such pairs as k in general will not be large. Note that
     * passed array is reseted to a new pointer. 
     */
    void get_top_k_results(boost::scoped_array<comm_chunk>& arr)const;
           
};


template<typename DataVec>
void 
knn_policy_base_data_handler<true>::fillin_majority_vote(const DataVec& labels){
    
#ifdef PARML_DEBUG
    
    using utilities::ExeLogicError;
    std::string msg = "In knn_policy_base_data_handler<true>::fillin_majority_vote(). k_distances empty ";
    Assert(k_distances.empty()==false,ExeLogicError(msg));
    msg = "In knn_policy_base_data_handler<true>::fillin_majority_vote(). Incompatible number of neighbors: "+
            std::to_string(k)+" and k_distances size: "+std::to_string(k_distances.size());
    Assert( k <= k_distances.size(),ExeLogicError(msg));
#endif
    
    //we loop ove all the k-distances as we want
    //the average
    for(size_type d=0; d<k; ++d){
        
        size_type row_idx = k_distances[d].first;
        real_type value = labels[row_idx];
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
    typedef size_type return_type;
    
    /**
     * @brief The type of the container that holds
     * the majority vote
     */
    typedef std::map<size_type,size_type> majority_vote_container_type;
    
    /**
     * @brief The type of the pair used for storing row-distance values
     */
    typedef std::pair<size_type,real_type> Pair;
    
    /**
     * @brief The type of the object to communicate when working with MPI.
     * The first index is the class index and the second the number of occurrences
     */
    typedef parframepp::serializable_pair<size_type,size_type> comm_chunk;
    
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
    size_type k;
    
    
    /**
      * @brief The top k distances computed by the classifier
      * coupled with row id that the distance occurred
      */
     
    std::vector<Pair> k_distances;
     
     
     /**
      * @brief The vector that holds the values of the labels
      * vector for the k-top neighbors
      */
    std::map<size_type,size_type> majority_vote;
    
     
     /**
      * @brief Constructor
      */
    knn_policy_base_data_handler(size_type k_);
     
     
     /**
       * @brief Given the labels vector and having computed the
       * the top k-distances store the values that correspond to these
       * top distances 
       */
    template<typename DataVec>
    void fillin_majority_vote(const DataVec& labels);
    
    
   /**
     * @brief Return the top-k results.  Return
     * a dynamic array of pairs (class idx,class counter). This
     * function is used in the implementation of MPI K-nn algorithm.
     * In general, we don't expect that there will be a large number
     * of such pairs as k in general will not be large. Note that
     * passed array is reseted to a new pointer. 
     */
    void get_top_k_results(boost::scoped_array<comm_chunk>& arr)const;
        
};



template<typename DataVec>
void 
knn_policy_base_data_handler<false>::fillin_majority_vote(const DataVec& labels){
    
#ifdef PARML_DEBUG
    
    using utilities::ExeLogicError;
    const std::string msg = "In knn_policy_base_data_handler<false>::fillin_majority_vote(). Number of neighbors: "+
                            std::to_string(k)+" not compatible with: "+
                            std::to_string(k_distances.size());
    //std::cout<<"k: "<<k<<" distances: "<<k_distances.size()<<std::endl;
    //Assert(k < k_distances.size(),ExeLogicError(msg));
#endif
    
    
   typedef std::map<size_type,size_type>::iterator iterator;
    
   for(size_type i=0; i<k; ++i){
        
       size_type idx = k_distances[i].first;
       size_type cls = labels[idx];

       utilities::add_or_update_map(majority_vote,cls,
                                    utilities::scalar_value_traits<size_type>::one(),
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
     * @brief Expose the type of the object needed for communication when using MPI
     */
    typedef typename knn_policy_base_data_handler<is_regressor>::comm_chunk comm_chunk;
    
     
    /**
     * @brief Return the value that indicates an invalid result
     */
    static return_type invalid_result(){return knn_policy_base_data_handler<is_regressor>::invalid_result_value;}
    
    /**
     * @brief Set the value for the policy that indicates an invalid result
     */
    static void set_invalid_result_value(return_type val){
        knn_policy_base_data_handler<is_regressor>::set_invalid_result_value(val);}
    
    /**
     * @brief Constructor
     */
    knn_policy_base(size_type k);
    
    
    /**
     * @brief Return the number of neighbors used for the classification
     */
    size_type n_neighbors()const{return data_handler_.k;}
    
    
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
    template<typename DataMat,typename DataVec,typename Similarity>
    void operator()(const utilities::DataInput<DataMat,DataVec>& data, 
                    const DataVec& input, const Similarity& sim,
                    const utilities::range1d<size_type>& range);
    
    
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
                              std::vector<std::pair<size_type,real_type> >&& distances);
    
    
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
    void resume(size_type k);
    
    /**
     * @brief Resume the object to a state just like when calling the constructor but
     * keep the number of neighbors
     */
    void resume(){resume(data_handler_.k);}
    
    
     /**
     * @brief Return the top-k results.  Return
     * a dynamic array of pairs. Depending on whether the implementation
     * is a regressor or a classifier it returns a dynamic array or pairs
     * (distance,value) or (class idx,class counter) respectively. This
     * function is used in the implementation of the MPI K-nn algorithm.
     * In general, we don't expect that there will be a large number
     * of such pairs as k in general will not be large. Note that
     * passed array is reseted to a new pointer. 
     */
    void get_top_k_results(boost::scoped_array<comm_chunk>& arr)const{return data_handler_.get_top_k_results(arr);}
    
protected:
    
    
    /**
     * @brief The data handler
     */
    knn_policy_base_data_handler<is_regressor> data_handler_;
    
};

template<bool is_regressor>
template<typename DataMat, typename DataVec,typename Similarity>
void 
knn_policy_base<is_regressor>::operator()(const utilities::DataInput<DataMat,DataVec>& data, 
                                          const DataVec& point, 
                                          const Similarity& sim,
                                          const utilities::range1d<size_type>& range){
    
    typedef typename knn_policy_base<is_regressor>::Pair Pair;
     
    //we will calculating as many distances as
    //the size of the range
    std::vector<Pair> distances;
    distances.reserve(range.size());
    
    for(size_type r=range.begin(); r<range.end(); ++r){
        
        //access the r-th row of the matrix
        const DataVec& data_point = data.t1.row(r);
        
        //compute the distance between the data point and the 
        //input point
        real_type dis = sim(data_point,point);
        
        //compute the similarity and append it to distances
        distances.push_back(std::make_pair(r,dis));  
    }
    
    //sort the computed pairs based on the distances
    std::sort(distances.begin(),distances.end(),
             [](const Pair& p1,const Pair& p2){
               return p1.second < p2.second;});
    
    {
        //empty what has been already computed 
        std::vector<Pair> empt;
        data_handler_.k_distances.swap(empt);
    }
               
    data_handler_.k_distances.reserve(data_handler_.k);
    
    
/*    
#ifdef PARML_DEBUG
    
    using utilities::ExeLogicError;
    const std::string msg = "In knn_class_policy::operator(). Number of neighbors: "+
                            std::to_string(data_handler_.k)+" not compatible with: "+
                            std::to_string(distances.size());
    
    Assert(data_handler_.k < distances.size(),ExeLogicError(msg));
#endif
*/
        
    for(size_type i=0; i<data_handler_.k; ++i){
       data_handler_.k_distances.push_back(distances[i]);
    }   
    
    //fill in the majority vote
    fillin_majority_vote(data.t2);
    
    /*if(compute_majority_vote_){
        auto y = DataInput::type_2(this->data_in_);
    
        kmeans_detail::fillin_majority_vote(majority_vote_,*y,distances,k_);
    }*/    
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
                                                    std::vector<std::pair<size_type,real_type> >&& distances){
    
    data_handler_.k_distances = std::move(distances);
    distances.clear();
    fillin_majority_vote(labels);   
}

    
}



#endif	/* KNN_POLICY_BASE_H */

