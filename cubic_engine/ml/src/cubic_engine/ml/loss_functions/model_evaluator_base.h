#ifndef MODEL_EVALUATOR_BASE_H
#define MODEL_EVALUATOR_BASE_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "boost/noncopyable.hpp"


namespace cengine{
namespace ml {

template<typename Model, typename DatasetTp>
class ModelEvaluatorBase: private boost::noncopyable
{
public:

    ///
    /// \brief model_t Type of the model
    ///
    typedef Model model_t;

    ///
    /// \brief dataset_t Type of the dataset
    ///
    typedef DatasetTp dataset_t;

    ///
    /// \brief row_t
    ///
    typedef typename DatasetTp::row_t row_t;

    ///
    /// \brief label_t
    ///
    typedef typename DatasetTp::label_value_t label_value_t;

    ///
    /// \brief
    ///
    virtual ~ModelEvaluatorBase()=default;

    ///
    /// \brief evaluate. Evaluate the underlying model under the given dataset.
    ///
    virtual real_t evaluate(const DatasetTp& dataset)const=0;

    ///
    /// \brief
    ///
    virtual real_t error_at(const row_t& p, const label_value_t& val)const = 0;

    ///
    /// \brief params_gradients. Compute the parameter gradients of the
    /// error metric on the given dataset
    ///
    virtual DynVec<real_t> params_gradients(const DatasetTp& dataset)const=0;

    ///
    /// \brief param_gradient_at
    ///
    virtual DynVec<real_t> param_gradient_at(const row_t& p, const label_value_t& label)const=0;

    ///
    ///
    ///
    virtual DynVec<real_t> parameters()const{return model_ptr_->coeffs();}

    ///
    /// \brief update_parameters
    /// \param params
    ///
    virtual void update_parameters(const DynVec<real_t>& params){model_ptr_->update_coeffs(params);}

    ///
    /// \brief n_parameters
    /// \return
    ///
    uint_t n_parameters()const noexcept{return model_ptr_ -> n_coeffs();}

protected:

    ///
    ///
    ///
    ModelEvaluatorBase() = default;

    ///
    /// \brief ModelEvaluatorBase
    ///
    explicit ModelEvaluatorBase(model_t& model);

    ///
    /// \brief model_ptr_ Pointer to the model to evaluate
    ///
    model_t* model_ptr_;


    model_t& model_ref_() noexcept{return *model_ptr_;}
    const model_t& model_ref_()const noexcept{return *model_ptr_;}

};

template<typename Model, typename DatasetTp>
ModelEvaluatorBase<Model, DatasetTp>::ModelEvaluatorBase(model_t& model)
        :
          model_ptr_(&model)
    {}


}
}

#endif // MODEL_EVALUATOR_BASE_H
