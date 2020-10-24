#ifndef DATA_SET_LOADERS_H
#define DATA_SET_LOADERS_H

#include "kernel/base/types.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include <utility>

namespace kernel
{

///
/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
///
std::pair<DynMat<real_t>, DynVec<real_t>> load_car_plant_dataset(uint_t label_idx=1, bool add_ones_column=true);

///
/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
///
std::pair<DynMat<real_t>, DynVec<real_t>> load_car_plant_multi_dataset(uint_t label_idx=2, bool add_ones_column=true);

/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<real_t>>> load_car_plant_multi_dataset_with_partitions(uint nparts,
                                                                                        uint_t label_idx=2,
                                                                                        bool add_ones_column=true);
///
/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
///
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<real_t>>> load_car_plant_dataset_with_partitions(uint nparts,
                                                                                  uint_t label_idx=1,
                                                                                  bool add_ones_column=true);

///
/// \brief Load the reduced iris data set
///
std::pair<DynMat<real_t>, DynVec<uint_t>> load_reduced_iris_data_set(bool add_ones_column=true);

///
/// \brief Load the reduced iris data set and assigned partitions
///
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<uint_t>>> load_reduced_iris_data_set_with_partitions(uint nparts,
                                                                                      bool add_ones_column=true);

///
/// \brief Load the iris data set and assigned
///
std::pair<DynMat<real_t>,
          DynVec<uint_t>> load_iris_data_set(bool add_ones_column=true);

///
/// \brief Load the reduced iris data set and assigned partitions
///
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<uint_t>>> load_iris_data_set_with_partitions(uint nparts,
                                                                              bool add_ones_column=true);

///
/// \brief Load the reduced iris data set
///
std::pair<DynMat<real_t>, DynVec<real_t>> load_x_y_sinuisoid_data_set(bool add_ones_column=true);


///
/// \brief Load the reduced iris data set and assigned partitions
///
std::pair<PartitionedType<DynMat<real_t>>,
          PartitionedType<DynVec<real_t>>> load_x_y_sinuisoid_data_set_with_partitions(uint nparts,
                                                                                      bool add_ones_column=true);

///
/// \brief Loads test data for Kmeans
/// Sample data set taken from scikit-learn example
/// which can be found here: https://scikit-learn.org/stable/modules/generated/sklearn.cluster.KMeans.html
///
DynMat<real_t> load_kmeans_test_data();

///
/// \brief load_wine_data_set. Loads the wine data set.
/// The wine data set is described here https://archive.ics.uci.edu/ml/datasets/wine
/// This is a data set with 178 rows and 13 columns. The attributes are mixed
/// integers and real numbers. The first column represents the class index and thus
/// saved separately and returned as DynVec<uint_t>
/// \param add_ones_column. Flag indicating whether a column of ones should be added in the
/// return matrix. If true then the matrix will be 178x13
///
std::pair<DynMat<real_t>,
          DynVec<uint_t>> load_wine_data_set(bool add_ones_column=true);

}

#endif // DATA_SET_LOADERS_H
