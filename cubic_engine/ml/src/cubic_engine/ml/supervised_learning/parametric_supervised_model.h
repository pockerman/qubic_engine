#ifndef PARAMETRIC_SUPERVISED_MODEL_H
#define PARAMETRIC_SUPERVISED_MODEL_H

#include "cubic_engine/ml/supervised_learning/supervised_model.h"

namespace cengine {
namespace ml {

template<typename DatasetTp, typename ValueTp>
class ParametricSupervisedModel: public SupervisedModel<DatasetTp, ValueTp>
{
public:

    ///
    /// \brief dataset_t. The type of the dataset
    ///
    typedef typename SupervisedModel<DatasetTp, ValueTp>::dataset_t dataset_t;

    ///
    /// \brief value_t The result value type
    ///
    typedef typename SupervisedModel<DatasetTp, ValueTp>::value_t value_t;

    ///
    /// \brief destructor
    ///
    virtual ~ParametricSupervisedModel()=default;

    ///
    /// \brief get_parameters. Returns the parameters of the model
    ///
    virtual std::vector<real_t> get_parameters()const=0;

    ///
    /// \brief update_parameters
    ///
    virtual void update_parameters(const std::vector<real_t>& parameters)=0;

protected:

    ///
    /// \brief Constructor
    ///
    ParametricSupervisedModel()=default;

};

}

}

#endif // PARAMETRIC_SUPERVISED_MODEL_H
