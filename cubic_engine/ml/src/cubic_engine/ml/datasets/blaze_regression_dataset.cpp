#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"
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

}
}
