#ifndef SERIAL_KNN_H
#define	SERIAL_KNN_H

#include "cubic_engine/ml/instance_learning/knn_control.h"
#include "cubic_engine/ml/instance_learning/knn_info.h"
#include "cubic_engine/ml/instance_learning/details/knn_classification_policy.h"

#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#include <utility>
#include <chrono>
#include <iostream>

namespace cengine{
namespace ml {


    
///
/// \brief Serial implementation
/// of K-nearest neighbors algorithm
///
template<typename DataSetType, typename Similarity>
class KnnClassifier
{
    
public:

     ///
     /// \brief Constructor
     ///
     KnnClassifier(const KnnControl& control);

	 ///
     /// \brief Train the model
	 ///
     void fit(const DataSetType& data_set);

	 ///
     /// \brief Predict outcome for the given vector
	 ///
     template<typename DataPoint>
     uint_t predict_one(const DataPoint& data);

	 ///
     /// \brief Predict outcome for the given dataset
	 ///
     DynVec<uint_t> predict_many(const DataSetType& data);

     ///
     /// \brief
     ///
     template<typename OtherDataSetTp>
     real_t score(const OtherDataSetTp& data)const;

private:

     ///
     /// \brief input_
     ///
     const KnnControl input_;

     ///
     /// \brief data_ptr_. Pointer to the data
     ///
     const DataSetType* data_ptr_;
};

template<typename DataSetType, typename Similarity>
KnnClassifier<DataSetType, Similarity>::KnnClassifier(const KnnControl& control)
    :
   input_(control),
   data_ptr_(nullptr)
{}


template<typename DataSetType, typename Similarity>
void
KnnClassifier<DataSetType, Similarity>::fit(const DataSetType& data_set){
    data_ptr_ = &data_set;
}

template<typename DataSetType, typename Similarity>
template<typename DataVec>
uint_t
KnnClassifier<DataSetType, Similarity>::predict_one(const DataVec& point){

#ifdef KERNEL_DEBUG
    assert(data_ptr_ != nullptr && "Model has not been trained");
    assert(input_.k != 0 && "Number of neighbors cannot be zero");
    assert(input_.k < data_ptr_->n_examples() && "Number of neighbors cannot be >= number of examples");
#endif

    uint_t k = input_.k;
    uint_t n_examples = data_ptr_->n_examples();

    // the type that will do the classification
    KnnClassificationPolicy actor(k);

    // the metric used for classification
    Similarity sim;
    
    //find the k smallest distances of
    //the given point from the given data set
    actor(*this->data_ptr_, point, sim);
    
    //get the result
    auto rslt = actor.get_result();
    return rslt;
}

template<typename DataSetType, typename Similarity>
DynVec<uint_t>
KnnClassifier<DataSetType, Similarity>::predict_many(const DataSetType& data){

#ifdef KERNEL_DEBUG
    assert(data_ptr_ != nullptr && "Model has not been trained");
    assert(input_.k != 0 && "Number of neighbors cannot be zero");
    assert(input_.k < data_ptr_->n_examples() && "Number of neighbors cannot be >= number of examples");
#endif

    uint_t k = input_.k;
    uint_t n_examples = data_ptr_->n_examples();

    // the type that will do the classification
    KnnClassificationPolicy actor(k);

    // the metric used for classification
    Similarity sim;

    DynVec<uint_t> result(data.n_examples());

    //find the k smallest distances of
    //the given point from the given data set

    for(uint row_idx=0; row_idx<data.n_examples(); ++row_idx){

        auto point = data.get_row(row_idx);

        actor(*this->data_ptr_, point, sim);

        //get the result
        auto rslt = actor.get_result();
        result[row_idx] = rslt;
    }

    return result;
}

template<typename DataSetType, typename Similarity>
template<typename OtherDataSetTp>
real_t
KnnClassifier<DataSetType, Similarity>::score(const OtherDataSetTp& data)const{

#ifdef KERNEL_DEBUG
    assert(data_ptr_ != nullptr && "Model has not been trained");
    assert(data_ptr_ ->n_features() == data.n_features() && "Invalid feature space size");
#endif

    real_t correctly_clss = 0;

    for(uint_t p=0; p < data.n_examples(); ++p){

        auto[point, label] = data[p];

        auto p_class = static_cast<uint_t>(predict_one(point));

        if(p_class == label){
            correctly_clss += 1.;
        }
    }

    return correctly_clss/data.size();
}
   
}
}


#endif	/* SERIAL_KNN_H */

