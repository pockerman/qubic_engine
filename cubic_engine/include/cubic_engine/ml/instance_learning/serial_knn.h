/* 
 * File:   serial_knn.h
 * Author: david
 *
 * Created on October 17, 2016, 8:19 PM
 */

#ifndef SERIAL_KNN_H
#define	SERIAL_KNN_H

#include "cubic_engine/ml/instance_learning/knn_input.h"
#include "cubic_engine/ml/instance_learning/knn_info.h"
#include "kernel/utilities/range_1d.h"

#include <utility>
#include <chrono>

namespace cengine
{
    
/**
 * @brief Serial implementation
 * of K-nearest neighbors algorithm
 */
template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
class Knn
{
    
public:

     /**
      * @brief Constructor
      */
     Knn(const KnnInput& input);

     /**
       * @brief Train the model
      */
     void train(const DataSetType& data_set, const LabelType& labels);

     /**
      * @brief Predict outcome for the given vector
      */
     template<typename DataPoint>
     std::pair<typename Actor::return_type, KnnInfo> predict(const DataPoint& data);

private:

      const DataSetType* data_ptr_;
      const LabelType* labels_ptr_;
      const KnnInput input_;

    
};

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
Knn<DataSetType, LabelType, Similarity, Actor>::Knn(const KnnInput& input)
    :
   input_(input)
{}


template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
void
Knn<DataSetType, LabelType, Similarity, Actor>::train(const DataSetType& data_set, const LabelType& labels){
    data_ptr_ = &data_set;
    labels_ptr_ = &labels;
}

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
template<typename DataVec>
std::pair<typename Actor::return_type,KnnInfo>
Knn<DataSetType, LabelType, Similarity, Actor>::predict(const DataVec& point){

    if(data_ptr_ == nullptr){
        throw std::logic_error("Dataset pointer in null");
    }

    if(labels_ptr_ == nullptr){
        throw std::logic_error("Labels pointer in null");
    }
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    uint_t k = input_.k;
    uint_t nrows = data_ptr_->rows();
    uint_t labels_size = labels_ptr_->size();

    if(labels_size != nrows){
        throw std::logic_error("Labels size: "+std::to_string(labels_size)+" does not match dataset rows: "+std::to_string(nrows));
    }

    if(k == 0 || k>= nrows){
        throw std::logic_error("Number of neighbors: "+std::to_string(k)+
                               " not in [1,"+std::to_string(nrows)+")");
    }
    
    KnnInfo info;
    info.n_neighbors = k;
    info.n_pts_predicted = 1;
    info.nprocs = 1;
    info.nthreads = 1;


    //for serial knn the range is all the
    //data set
    kernel::range1d<uint_t> r(0,nrows);

    // the type that will do the classification
    Actor actor(k);

    // the metric used for classification
    Similarity sim;
    
    //find the k smallest distances of
    //the given point from the given data set
    actor(*this->data_ptr_, *this->labels_ptr_, point, sim, r);
    
    //get the result
    auto rslt = actor.get_result();
    
    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    
    return std::make_pair(rslt,info);
}
   
}



#endif	/* SERIAL_KNN_H */

