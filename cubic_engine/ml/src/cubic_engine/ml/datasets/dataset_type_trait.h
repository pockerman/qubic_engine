#ifndef DATASET_TYPE_TRAIT_H
#define DATASET_TYPE_TRAIT_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine {
namespace ml {

class RegressionDatasetBase;

template<typename T>
struct dataset_t_trait;

template<>
struct dataset_t_trait<real_t>
{
    typedef RegressionDatasetBase type;
};

}

}

#endif // DATASET_TYPE_TRAIT_H
