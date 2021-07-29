#include "cubic_engine/ml/datasets/regression_dataset_base.h"

namespace cengine{
namespace ml {

std::vector<std::string>
RegressionDatasetBase::get_columns()const{

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
