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
    bool empty()const{return examples_.rows() == 0;}

    ///
    /// \brief n_rows
    /// \return
    ///
    uint_t n_rows()const{return examples_.rows();}

    ///
    /// \brief n_features
    /// \return
    ///
    virtual uint_t n_features()const override{return examples_.columns();}

    ///
    /// \brief n_examples
    ///
    virtual uint_t n_examples()const override{return examples_.rows();}

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
    /// \brief get_features
    ///
    virtual std::any get_features()const override{return std::any(examples_);}

    ///
    /// \brief get_labels
    /// \return
    ///
    virtual std::any get_labels()const override{return std::any(labels_);}

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
    storage_engine_t examples_;


    ///
    /// \brief labels_
    ///
    labels_t labels_;

};

}
}

#endif // REGRESSION_DATASET_H
