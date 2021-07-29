#ifndef REGRESSION_DATASET_H
#define REGRESSION_DATASET_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/regression_dataset_base.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <map>

namespace cengine {
namespace ml{

///
/// \brief The BlazeRegressionDataset class
///
class BlazeRegressionDataset: private boost::noncopyable
{
public:

    ///
    /// \brief storage_engine_t
    ///
    typedef DynMat<real_t> storage_engine_t;

    ///
    /// \brief storage_engine_t
    ///
    typedef DynMat<real_t> features_t;

    ///
    /// \brief labels_t
    ///
    typedef DynVec<real_t> labels_t;

    ///
    /// \brief row_t
    ///
    typedef DynVec<real_t> row_t;

    ///
    /// \brief RegressionDataset
    ///
    BlazeRegressionDataset();

    ///
    /// \brief empty
    /// \return
    ///
    bool empty()const noexcept{return examples_.rows() == 0;}

    ///
    /// \brief n_rows
    /// \return
    ///
    uint_t n_rows()const noexcept{return examples_.rows();}

    ///
    /// \brief n_features
    /// \return
    ///
    uint_t n_features()const{return examples_.columns();}

    ///
    /// \brief n_examples
    ///
    uint_t n_examples()const{return examples_.rows();}

    ///
    /// \brief feature_matrix
    /// \return
    ///
    const storage_engine_t& feature_matrix()const{return examples_;}

    ///
    /// \brief labels
    /// \return
    ///
    const labels_t& labels()const{return labels_;}

    ///
    /// \brief feature_matrix
    /// \return
    ///
    storage_engine_t& feature_matrix(){return examples_;}

    ///
    /// \brief labels
    /// \return
    ///
    labels_t& labels(){return labels_;}

    ///
    /// \brief load_from_file
    ///
    template<typename FileReader>
    void load_from_file(FileReader& reader);

    ///
    /// \brief Load the dataset from the given loader
    ///
    template<typename DataLoader>
    void load_from_loader(const DataLoader& loader);

    ///
    /// \brief load_from_data
    /// \param features
    /// \param labels
    ///
    void load_from_data(const features_t& features, const labels_t& labels);

    ///
    /// \brief get_columns
    /// \return
    ///
    std::vector<std::string> get_columns()const;

    ///
    /// \brief set_columns
    /// \param columns
    ///
    void set_columns(const std::vector<std::string>& columns);

    ///
    /// \brief columns
    ///
    auto columns()-> std::map<std::string, uint_t>&{return columns_;}

    ///
    /// \brief columns
    ///
    auto columns()const-> const std::map<std::string, uint_t>&{return columns_;}

private:


    ///
    /// \brief features_ The features matrix
    ///
    storage_engine_t examples_;


    ///
    /// \brief labels_
    ///
    labels_t labels_;

    ///
    ///
    ///
    std::map<std::string, uint_t> columns_;

};


template<typename DataLoader>
void
BlazeRegressionDataset::load_from_loader(const DataLoader& loader){
    loader.load(examples_, labels_, columns());
}

template<typename FileReader>
void
BlazeRegressionDataset::load_from_file(FileReader& reader){
    reader.read(examples_, labels_, columns());
}

}
}

#endif // REGRESSION_DATASET_H
