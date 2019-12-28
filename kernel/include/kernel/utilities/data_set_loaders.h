#ifndef DATA_SET_LOADERS_H
#define DATA_SET_LOADERS_H

#include "kernel/base/types.h"
#include <utility>
namespace kernel
{

/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
std::pair<DynMat<real_t>, DynVec<real_t>> load_car_plant_dataset(uint_t label_idx=1, bool add_ones_column=true);


}

#endif // DATA_SET_LOADERS_H
