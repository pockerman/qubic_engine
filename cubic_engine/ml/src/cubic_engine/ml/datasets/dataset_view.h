#ifndef DATASET_VIEW_H
#define DATASET_VIEW_H

#include "kernel/base/config.h"
#include "cubic_engine/base/cubic_engine_types.h"



#include <vector>
#include <tuple>

namespace cengine {
namespace ml {


template<typename DatasetType>
class DatasetView
{
public:

    ///
    /// \brief label_value_t
    ///
    typedef typename DatasetType::label_value_t label_value_t;

    ///
    /// \brief row_t
    ///
    typedef typename DatasetType::row_t row_t;

    ///
    /// \brief DatasetView
    /// \param dataset
    /// \param indices
    ///
    explicit DatasetView(const DatasetType& dataset, const std::vector<uint_t>& indices=std::vector<uint_t>());

    ///
    /// \brief empty
    /// \return
    ///
    bool empty()const noexcept{return dataset_ptr_ != nullptr;}

    ///
    /// \brief n_features
    /// \return
    ///
    uint_t n_features()const{return dataset_ptr_ -> n_features();}

    ///
    /// \brief n_examples
    ///
    uint_t n_examples()const{return dataset_ptr_ ->n_examples();}

    ///
    /// \brief operator []
    /// \param i
    /// \return
    ///
    std::tuple<row_t, label_value_t> operator[](uint_t i)const;

private:


    ///
    /// \brief dataset_ptr_
    ///
    const DatasetView* dataset_ptr_;

    ///
    /// \brief view_idices_
    ///
    std::vector<uint_t> view_idices_;

};

template<typename DatasetType>
DatasetView<DatasetType>::DatasetView(const DatasetType& dataset, const std::vector<uint_t>& indices)
    :
     dataset_ptr_(&dataset),
     view_idices_(indices)
{}


template<typename DatasetType>
std::tuple<typename DatasetView<DatasetType>::row_t, typename DatasetView<DatasetType>::label_value_t>
DatasetView<DatasetType>::operator[](uint_t i)const{

#ifdef KERNEL_DEBUG
    auto idx = view_idices_.at(i);
#else
    auto idx = view_idices_[i];
#endif
    return dataset_ptr_->operator[](idx);
}


}

}

#endif // DATASET_VIEW_H
