#ifndef SUPERVISED_MODEL_H
#define SUPERVISED_MODEL_H

#include "cubic_engine/ml/supervised_learning/supervised_model_type.h"
#include <vector>

namespace cengine {
namespace ml {

///
/// \brief Base class for supervised models
///
template<typename DatasetTp, typename ValueTp>
class SupervisedModel
{

public:

    ///
    /// \brief dataset_t. The type of the dataset
    ///
    typedef DatasetTp dataset_t;

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
    //virtual void train(const DatasetTp& data) = 0;

    ///
    /// \brief predict
    ///
    virtual value_t predict(const dataset_t& data)const=0;

    ///
    /// \brief predict
    ///
    virtual std::vector<value_t> predict_many(const dataset_t& data)const=0;

protected:

    ///
    /// \brief Constructor
    ///
    SupervisedModel()=default;

};

}

}

#endif // SUPERVISED_MODEL_H
