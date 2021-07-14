#ifndef REGRESSION_DATASET_BASE_H
#define REGRESSION_DATASET_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/dataset_base.h"
#include "cubic_engine/ml/datasets/storage_engine_type.h"

#include <string>
#include <map>
#include <any>

namespace cengine {
namespace ml {

class RegressionDatasetBase: public DatasetBase
{
public:

    ///
    /// \brief Destructor
    ///
    virtual ~RegressionDatasetBase() {}

    ///
    /// \brief get_storage_type
    /// \return
    ///
    DataStorageEngineType get_storage_type() const{return storage_type_;}

    ///
    /// \brief get_columns
    /// \return
    ///
    std::vector<std::string> get_columns()const{}

    ///
    /// \brief set_columns
    /// \param columns
    ///
    void set_columns(const std::vector<std::string>& columns){}

    ///
    /// \brief load_from_file
    ///
    virtual void load_from_file(const std::string& filename) = 0;

    ///
    /// \brief n_features
    ///
    virtual uint_t n_features()const = 0;

    ///
    /// \brief n_examples
    ///
    virtual uint_t n_examples()const = 0;

    ///
    /// \brief get_features. This call creates a copy of the
    /// features so it may be expensive
    ///
    virtual std::any get_features()const=0;

    ///
    /// \brief get_labels. This call creates a copy of the
    /// labels so it may be expensive
    /// \return
    ///
    virtual std::any get_labels()const=0;

protected:

    ///
    /// \brief
    ///
    RegressionDatasetBase(DataStorageEngineType storage_type);

    ///
    /// \brief storage_type_
    ///
    DataStorageEngineType storage_type_;

    ///
    ///
    ///
    std::map<std::string, uint_t> columns_;


};

inline
RegressionDatasetBase::RegressionDatasetBase(DataStorageEngineType storage_type)
    :
      DatasetBase(),
      storage_type_(storage_type),
      columns_()
{}

}

}

#endif // REGRESSION_DATASET_BASE_H
