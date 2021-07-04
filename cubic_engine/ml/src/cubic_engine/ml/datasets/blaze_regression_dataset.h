#ifndef REGRESSION_DATASET_H
#define REGRESSION_DATASET_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/regression_dataset_base.h"

#include <string>
#include <map>

namespace cengine {
namespace ml{

///
/// \brief The BlazeRegressionDataset class
///
class BlazeRegressionDataset: public RegressionDatasetBase
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
    bool empty()const{return features_.rows() == 0;}

    ///
    /// \brief n_rows
    /// \return
    ///
    uint_t n_rows()const{return features_.rows();}

    ///
    /// \brief n_features
    /// \return
    ///
    uint_t n_features()const{return features_.columns();}

    ///
    /// \brief feature_matrix
    /// \return
    ///
    const storage_engine_t& feature_matrix()const{return features_;}

    ///
    /// \brief load_from_file
    ///
    virtual void load_from_file(const std::string& filename) override;

    ///
    /// \brief load_from_data
    /// \param features
    /// \param labels
    ///
    void load_from_data(const features_t& features, const labels_t& labels);

private:


    ///
    /// \brief features_ The features matrix
    ///
    storage_engine_t features_;

    ///
    /// \brief labels_
    ///
    DynVec<real_t> labels_;



};

}
}

#endif // REGRESSION_DATASET_H
