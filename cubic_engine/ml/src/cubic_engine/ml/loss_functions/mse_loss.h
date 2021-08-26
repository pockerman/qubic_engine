#ifndef MSE_LOSS_H
#define MSE_LOSS_H

#include "cubic_engine/ml/loss_functions/model_evaluator_base.h"
#include "cubic_engine/ml/loss_functions/sse_loss.h"

namespace cengine {
namespace ml {

template<typename Model, typename DatasetTp>
class MSELoss: public ModelEvaluatorBase<Model, DatasetTp>
{

public:

    ///
    /// \brief model_t Type of the model
    ///
    typedef typename ModelEvaluatorBase<Model, DatasetTp>::model_t model_t;

    ///
    /// \brief dataset_t Type of the dataset
    ///
    typedef typename ModelEvaluatorBase<Model, DatasetTp>::dataset_t dataset_t;

    ///
    /// \brief row_t
    ///
    typedef typename ModelEvaluatorBase<Model, DatasetTp>::row_t row_t;

    ///
    /// \brief label_t
    ///
    typedef typename ModelEvaluatorBase<Model, DatasetTp>::label_value_t label_value_t;

    ///
    ///
    ///
    explicit MSELoss(model_t& model);

    ///
    /// \brief evaluate. Evaluate the underlying model under the given dataset.
    ///
    virtual real_t evaluate(const DatasetTp& dataset)const override final;

    ///
    /// \brief
    ///
    virtual real_t error_at(const row_t& p, const label_value_t& val)const override final{return sse_loss_.error_at(p, val);};

    ///
    /// \brief params_gradients. Compute the parameter gradients of the
    /// error metric on the given dataset
    ///
    virtual DynVec<real_t> params_gradients(const DatasetTp& dataset)const override final;

    ///
    /// \brief param_gradient_at
    /// \param p
    /// \param label
    /// \return
    ///
    virtual DynVec<real_t> param_gradient_at(const row_t& p, const label_value_t& label)const override final{return sse_loss_.param_gradient_at(p, label);}

private:

    SSELoss<model_t, dataset_t> sse_loss_;

};

template<typename Model, typename DatasetTp>
MSELoss<Model, DatasetTp>::MSELoss(model_t& model)
    :
     ModelEvaluatorBase<Model, DatasetTp>(model),
     sse_loss_(model)
{}

template<typename Model, typename DatasetTp>
real_t
MSELoss<Model, DatasetTp>::evaluate(const DatasetTp& dataset)const{

    return sse_loss_.evaluate(dataset) / dataset.n_examples();
}

template<typename Model, typename DatasetTp>
DynVec<real_t>
MSELoss<Model, DatasetTp>::params_gradients(const DatasetTp& dataset)const{

    return sse_loss_.params_gradients(dataset) / dataset.n_examples();
}

}

}

#endif // MSE_LOSS_H
