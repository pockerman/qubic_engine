#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
#include "kernel/base/config.h"
#include "kernel/maths/matrix_traits.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif
#include <algorithm>

namespace cengine{
namespace ml{

BlazeRegressionDataset::BlazeRegressionDataset()
    :
      examples_(),
      labels_(),
      columns_()
{}


void
BlazeRegressionDataset::load_from_data(const features_t& features, const labels_t& labels){

    examples_ = std::move(features);
    labels_ = std::move(labels);

}

std::vector<std::string>
BlazeRegressionDataset::get_columns()const{

    if(columns_.empty()){
        return std::vector<std::string>();
    }

    std::vector<std::string> keys;
    keys.reserve(columns_.size());

    std::for_each(columns_.begin(), columns_.end(),
                  [&](auto itr){keys.push_back(itr.first);});
    return keys;

}

std::tuple<BlazeRegressionDataset::row_t, BlazeRegressionDataset::label_value_t>
BlazeRegressionDataset::operator[](uint_t i)const{

#ifdef KERNEL_DEBUG
    assert( i < examples_.rows() && "Invalid row index specified.");
#endif

    return {kernel::matrix_row_trait<DynMat<real_t>>::get_row(examples_, i), labels_[i]};
}

}
}
