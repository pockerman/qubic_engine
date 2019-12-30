#ifndef DATA_SET_LOADERS_H
#define DATA_SET_LOADERS_H

#include "kernel/base/types.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include <utility>
namespace kernel
{

/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
std::pair<DynMat<real_t>, DynVec<real_t>> load_car_plant_dataset(uint_t label_idx=1, bool add_ones_column=true);


/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
std::pair<DynMat<real_t>, DynVec<real_t>> load_car_plant_multi_dataset(uint_t label_idx=2, bool add_ones_column=true);

/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<real_t>>> load_car_plant_multi_dataset_with_partitions(uint nparts,
                                                                                        uint_t label_idx=2,
                                                                                        bool add_ones_column=true);


/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<real_t>>> load_car_plant_dataset_with_partitions(uint nparts, uint_t label_idx=1, bool add_ones_column=true);




}

#endif // DATA_SET_LOADERS_H
