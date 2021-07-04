#include "cubic_engine/ml/datasets/blaze_regression_dataset.h"

namespace cengine{
namespace ml{

BlazeRegressionDataset::BlazeRegressionDataset()
    :
      RegressionDatasetBase(DataStorageEngineType::BLAZE_MAT),
      features_(),
      labels_()
{}

void
BlazeRegressionDataset::load_from_file(const std::string& filename){

}


void
BlazeRegressionDataset::load_from_data(const features_t& features, const labels_t& labels){

}



}
}
