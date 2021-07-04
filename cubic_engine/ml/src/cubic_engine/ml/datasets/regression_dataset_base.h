#ifndef REGRESSION_DATASET_BASE_H
#define REGRESSION_DATASET_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/dataset_base.h"
#include "cubic_engine/ml/datasets/storage_engine_type.h"

#include <string>
#include <map>

namespace cengine {
namespace ml {

class RegressionDatasetBase: public DatasetBase
{
public:

    ///
    /// \brief Destructor
    ///
    virtual ~RegressionDatasetBase() = default;

    ///
    /// \brief get_storage_type
    /// \return
    ///
    DataStorageEngineType get_storage_type() const{return storage_type_;}

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
    /// \brief load_from_file
    ///
    virtual void load_from_file(const std::string& filename);


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

}

}

#endif // REGRESSION_DATASET_BASE_H
