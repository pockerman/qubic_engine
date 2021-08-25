#ifndef KNN_REGRESSOR_H
#define KNN_REGRESSOR_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/instance_learning/knn_control.h"
#include "cubic_engine/ml/instance_learning/details/knn_average_regression_policy.h"

#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif


namespace cengine {
namespace ml {

///
/// \brief Serial implementation
/// of K-nearest neighbors algorithm
///
template<typename DataSetType, typename Similarity>
class KnnRegressor
{

public:

     ///
     /// \brief Constructor
     ///
     explicit KnnRegressor(const KnnControl& control);

     ///
     /// \brief Train the model
     ///
     void fit(const DataSetType& data_set);

     ///
     /// \brief Predict outcome for the given vector
     ///
     template<typename DataPoint>
     real_t predict_one(const DataPoint& data)const;

     ///
     /// \brief Predict outcome for the given dataset
     ///
     DynVec<real_t> predict_many(const DataSetType& data)const;

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
KnnRegressor<DataSetType, Similarity>::KnnRegressor(const KnnControl& control)
    :
   input_(control),
   data_ptr_(nullptr)
{}


template<typename DataSetType, typename Similarity>
void
KnnRegressor<DataSetType, Similarity>::fit(const DataSetType& data_set){
    data_ptr_ = &data_set;
}

template<typename DataSetType, typename Similarity>
template<typename DataVec>
real_t
KnnRegressor<DataSetType, Similarity>::predict_one(const DataVec& point)const{

#ifdef KERNEL_DEBUG
    assert(data_ptr_ != nullptr && "Model has not been trained");
    assert(input_.k != 0 && "Number of neighbors cannot be zero");
    assert(input_.k < data_ptr_->n_examples() && "Number of neighbors cannot be >= number of examples");
#endif

    uint_t k = input_.k;
    uint_t n_examples = data_ptr_->n_examples();

    // the type that will do the classification
    KnnAvgRegressionPolicy actor(k);

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
DynVec<real_t>
KnnRegressor<DataSetType, Similarity>::predict_many(const DataSetType& data)const{

#ifdef KERNEL_DEBUG
    assert(data_ptr_ != nullptr && "Model has not been trained");
    assert(input_.k != 0 && "Number of neighbors cannot be zero");
    assert(input_.k < data_ptr_->n_examples() && "Number of neighbors cannot be >= number of examples");
#endif

    uint_t k = input_.k;
    uint_t n_examples = data_ptr_->n_examples();

    // the type that will do the classification
    KnnAvgRegressionPolicy actor(k);

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

}
}

#endif // KNN_REGRESSOR_H
