#ifndef DATASET_SPLITS_H
#define DATASET_SPLITS_H

#include "kernel/base/config.h"
#include "cubic_engine/ml/datasets/dataset_view.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

#include <random>
#include <tuple>

namespace cengine {
namespace ml {

template<typename DatasetType>
std::tuple<DatasetView<DatasetType>, DatasetView<DatasetType>> test_train_split(const DatasetType& dataset, real_t test_size, uint_t seed){

#ifdef KERNEL_DEBUG
    assert(!dataset.empty() && "Cannot split empty dataset");
#endif

    DatasetView<DatasetType> train(dataset);
    DatasetView<DatasetType> test(dataset);

    return {train, test};
}




}

}

#endif // DATASET_SPLITS_H
