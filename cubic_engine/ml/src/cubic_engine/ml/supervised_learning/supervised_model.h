#ifndef SUPERVISED_MODEL_H
#define SUPERVISED_MODEL_H

#include "cubic_engine/ml/supervised_learning/supervised_model_type.h"
#include "cubic_engine/ml/datasets/dataset_type_trait.h"

#include <vector>
#include <map>
#include <any>

namespace cengine {
namespace ml {

///
///
///
class DatasetBase;

///
/// \brief Base class for supervised models
///
template<typename ValueTp>
class SupervisedModel
{

public:

    ///
    /// \brief dataset_t. The type of the dataset
    ///
    typedef typename dataset_t_trait<ValueTp>::type dataset_t;

    ///
    /// \brief value_t The result value type
    ///
    typedef ValueTp value_t;

    ///
    /// \brief Destructor
    ///
    virtual ~SupervisedModel() = default;

    ///
    /// \brief train. Train the supervised model on the
    /// provided dataset
    ///
    virtual void fit(const dataset_t& data, const std::map<std::string, std::any>& options) = 0;

    ///
    /// \brief predict
    ///
    virtual value_t predict(const DatasetBase& data)const=0;

    ///
    /// \brief predict
    ///
    virtual std::vector<value_t> predict_many(const DatasetBase& data)const=0;


protected:

    ///
    /// \brief Constructor
    ///
    SupervisedModel();

};

template<typename ValueTp>
SupervisedModel<ValueTp>::SupervisedModel()
{}


}

}

#endif // SUPERVISED_MODEL_H
