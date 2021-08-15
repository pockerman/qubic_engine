#ifndef SSE_LOSS_H
#define SSE_LOSS_H

#include "cubic_engine/ml/loss_functions/model_evaluator_base.h"
#include "cubic_engine/ml/loss_functions/squared_sum.h"
#include "kernel/maths/functions/dummy_function.h"

namespace cengine{
namespace ml{


template<typename Model, typename DatasetTp, typename RegularizerFn=kernel::DummyFunction<real_t, DatasetTp, DynVec<real_t>>>
class SSELoss: public ModelEvaluatorBase<Model, DatasetTp>
{
public:

    ///
    /// \brief model_t Type of the model
    ///
    typedef typename ModelEvaluatorBase<Model, DatasetTp>::model_t model_t;

    ///
    /// \brief dataset_t Type of the dataset
    ///
    typedef typename ModelEvaluatorBase<Model, DatasetTp>::model_t dataset_t;

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
    explicit SSELoss(model_t& model);

    ///
    /// \brief evaluate. Evaluate the underlying model under the given dataset.
    ///
    virtual real_t evaluate(const DatasetTp& dataset)const override final;

    ///
    /// \brief
    ///
    virtual real_t error_at(const row_t& p, const label_value_t& val)const override final;

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
    virtual DynVec<real_t> param_gradient_at(const row_t& p, const label_value_t& label)const override final;

};

template<typename Model, typename DatasetTp, typename RegularizerFn>
SSELoss<Model, DatasetTp, RegularizerFn>::SSELoss(model_t& model)
    :
   ModelEvaluatorBase<Model, DatasetTp>(model)
{}


template<typename Model, typename DatasetTp, typename RegularizerFn>
real_t
SSELoss<Model, DatasetTp, RegularizerFn>::evaluate(const DatasetTp& dataset)const{

    const auto& model = this->model_ref_();

    static auto func = [&model](const auto& row, const auto& /*label*/)->real_t{ return model.value_at(row);};
    auto result = dataset.template accumulate<decltype (func)>( func, dataset.n_examples());

    const auto& labels = dataset.labels();
    SqrAccumulator<MinusT> sqr_sum;
    auto err = sqr_sum(labels, result);
    return err;
}

template<typename Model, typename DatasetTp, typename RegularizerFn>
real_t
SSELoss<Model, DatasetTp, RegularizerFn>::error_at(const row_t& p, const label_value_t& val)const{

    auto model_val = this->model_ref_().value_at(p);
    return (val - model_val)*(val - model_val);

}

template<typename Model, typename DatasetTp, typename RegularizerFn>
DynVec<real_t>
SSELoss<Model, DatasetTp, RegularizerFn>::params_gradients(const DatasetTp& dataset)const{

    DynVec<real_t> gradients(this->model_ref_().n_coeffs(), 0.0);

    dataset.iterate([this, &gradients](const auto& row, const auto& label){
        auto grad = this->param_gradient_at(row, label);
        gradients += grad;
    });

    return gradients;
}

template<typename Model, typename DatasetTp, typename RegularizerFn>
DynVec<real_t>
SSELoss<Model, DatasetTp, RegularizerFn>::param_gradient_at(const row_t& p, const label_value_t& label)const{

    DynVec<real_t> grad(this->model_ref_().n_coeffs(), 0.0);

    auto errorat = error_at(p, label);
    auto model_grads = this->model_ref_().param_grads_at(p);

    for(int coeff=0; coeff<this->model_ref_().n_coeffs(); ++coeff){
         grad[coeff] = -2.0*errorat*model_grads[coeff];
    }

    return grad;
}


}
}

#endif // SSE_LOSS_H
