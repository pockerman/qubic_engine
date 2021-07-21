#ifndef DATA_SET_LOADERS_H
#define DATA_SET_LOADERS_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/parallel/utilities/partitioned_type.h"
#include <utility>

namespace cengine{
namespace ml {



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


///
/// \brief load_car_plant_multi_dataset
/// \param mat
/// \param labels
/// \param label_idx
/// \param add_ones_column
///
void load_car_plant_multi_dataset(DynMat<real_t>& mat, DynVec<real_t>& labels,
                                  uint_t label_idx=2, bool add_ones_column=true);

///
/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
///
std::pair<kernel::PartitionedType<DynMat<real_t>>,
          kernel::PartitionedType<DynVec<real_t>>> load_car_plant_multi_dataset_with_partitions(uint nparts,
                                                                                        uint_t label_idx=2,
                                                                                        bool add_ones_column=true);
///
/// \brief The car plant data set is a simple dataset with two columns
/// of real type and 12 observations
///
std::pair<kernel::PartitionedType<DynMat<real_t>>,
          kernel::PartitionedType<DynVec<real_t>>> load_car_plant_dataset_with_partitions(uint nparts,
                                                                                  uint_t label_idx=1,
                                                                                  bool add_ones_column=true);

///
/// \brief Load the reduced iris data set
///
std::pair<DynMat<real_t>, DynVec<uint_t>> load_reduced_iris_data_set(bool add_ones_column=true);

///
/// \brief Load the reduced iris data set and assigned partitions
///
std::pair<kernel::PartitionedType<DynMat<real_t>>,
          kernel::PartitionedType<DynVec<uint_t>>> load_reduced_iris_data_set_with_partitions(uint nparts,
                                                                                      bool add_ones_column=true);

///
/// \brief Load the iris data set and assigned
///
std::pair<DynMat<real_t>,
          DynVec<uint_t>> load_iris_data_set(bool add_ones_column=true);



///
/// \brief Load the reduced iris data set and assigned partitions
///
std::pair<kernel::PartitionedType<DynMat<real_t>>,
          kernel::PartitionedType<DynVec<uint_t>>> load_iris_data_set_with_partitions(uint nparts,
                                                                              bool add_ones_column=true);

///
/// \brief Load the sinuisoidal data set
///
std::pair<DynMat<real_t>, DynVec<real_t>> load_x_y_sinuisoid_data_set(bool add_ones_column=true);


///
/// \brief Load the reduced iris data set and assigned partitions
///
std::pair<kernel::PartitionedType<DynMat<real_t>>,
          kernel::PartitionedType<DynVec<real_t>>> load_x_y_sinuisoid_data_set_with_partitions(uint nparts,
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
		  

///
/// \brief Load a random set
///
void load_random_set_one(DynMat<real_t>& matrix);

}
}

#endif // DATA_SET_LOADERS_H
