/* 
 * File:   serial_knn.h
 * Author: david
 *
 * Created on October 17, 2016, 8:19 PM
 */

#ifndef SERIAL_KNN_H
#define	SERIAL_KNN_H

#include "cubic_engine/ml/instance_learning/utils/knn_control.h"
#include "cubic_engine/ml/instance_learning/utils/knn_info.h"
#include "kernel/utilities/range_1d.h"
#include "kernel/maths/matrix_utilities.h"

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

     /// \brief
     typedef KnnInfo output_t;

     /// \brief The return type
     typedef typename Actor::return_t return_t;

     /// \brief Constructor
     Knn(const KnnControl& control);

     /// \brief Train the model
     void train(const DataSetType& data_set, const LabelType& labels);

     /// \brief Predict outcome for the given vector
     template<typename DataPoint>
     std::pair<return_t, output_t> predict(const DataPoint& data);

     /// \brief Predict outcome for the given dataset
     std::pair<std::vector<return_t>, output_t> predict(const DataSetType& data);

private:

      const KnnControl input_;
      const DataSetType* data_ptr_;
      const LabelType* labels_ptr_;
};

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
Knn<DataSetType, LabelType, Similarity, Actor>::Knn(const KnnControl& control)
    :
   input_(control),
   data_ptr_(nullptr),
   labels_ptr_(nullptr)
{}


template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
void
Knn<DataSetType, LabelType, Similarity, Actor>::train(const DataSetType& data_set, const LabelType& labels){
    data_ptr_ = &data_set;
    labels_ptr_ = &labels;
}

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
template<typename DataVec>
std::pair<typename Knn<DataSetType, LabelType, Similarity, Actor>::return_t,
          typename Knn<DataSetType, LabelType, Similarity, Actor>::output_t>
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
    kernel::range1d<uint_t> r(0, nrows);

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

template<typename DataSetType, typename LabelType, typename Similarity, typename Actor>
std::pair<std::vector<typename Knn<DataSetType, LabelType, Similarity, Actor>::return_t>,
          typename Knn<DataSetType, LabelType, Similarity, Actor>::output_t>
Knn<DataSetType, LabelType, Similarity, Actor>::predict(const DataSetType& data){

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
    kernel::range1d<uint_t> range(0, nrows);

    // the type that will do the classification
    Actor actor(k);

    // the metric used for classification
    Similarity sim;

    std::vector<typename Knn<DataSetType, LabelType, Similarity, Actor>::return_t> result(data.rows());

    //find the k smallest distances of
    //the given point from the given data set

    for(uint row_idx=0; row_idx<data.rows(); ++row_idx){

        auto point = kernel::get_row(data, row_idx);
        actor(*this->data_ptr_, *this->labels_ptr_, point, sim, range);

        //get the result
        auto rslt = actor.get_result();
        result[row_idx] = rslt;
    }



    end = std::chrono::system_clock::now();
    info.runtime = end-start;
    return std::make_pair(result,info);

}
   
}



#endif	/* SERIAL_KNN_H */

