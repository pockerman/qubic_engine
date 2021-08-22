#ifndef CATEGORICAL_CROSS_ENTROPY_H
#define CATEGORICAL_CROSS_ENTROPY_H

#include "cubic_engine/ml/loss_functions/model_evaluator_base.h"
#include "cubic_engine/ml/loss_functions/mse_loss.h" // for calculating gradients
#include "kernel/base/kernel_consts.h"
#include <cmath>

namespace cengine {
namespace ml {

///
/// \brief CategoricalCrossEntropy class. For more information
/// check: https://ml-cheatsheet.readthedocs.io/en/latest/loss_functions.html
/// see also: https://stats.stackexchange.com/questions/250937/which-loss-function-is-correct-for-logistic-regression
///
///
template<typename Model, typename DatasetTp>
class CategoricalCrossEntropy: public ModelEvaluatorBase<Model, DatasetTp>
{
public:

    // \brief model_t Type of the model
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
    /// \brief Constructor
    ///
    explicit CategoricalCrossEntropy(model_t& model, uint_t n_classes=2);

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
    ///
    virtual DynVec<real_t> param_gradient_at(const row_t& p, const label_value_t& label)const override final;


private:

    ///
    /// \brief n_classes_
    ///
    uint_t n_classes_;

    ///
    /// \brief two_class_error_value_
    /// \param dataset
    /// \return
    ///
    real_t two_class_error_value_(const DatasetTp& dataset)const;

};

template<typename Model, typename DatasetTp>
CategoricalCrossEntropy<Model, DatasetTp>::CategoricalCrossEntropy(model_t& model, uint_t n_classes)
    :
     ModelEvaluatorBase<Model, DatasetTp>(model),
     n_classes_(n_classes)
{}

template<typename Model, typename DatasetTp>
real_t
CategoricalCrossEntropy<Model, DatasetTp>::evaluate(const DatasetTp& dataset)const{

    if(n_classes_ == 2){
        return two_class_error_value_(dataset);
    }

    return 0.0;
}

template<typename Model, typename DatasetTp>
real_t
CategoricalCrossEntropy<Model, DatasetTp>::two_class_error_value_(const DatasetTp& dataset)const{

    auto result = 0.0;
    for(uint_t e=0; e < dataset.n_examples(); ++e){

        auto [features, label] = dataset[e];

        auto prediction = this->model_ref_().value_at(features);

        //h is close to one
        if(std::fabs(1 - prediction) < kernel::KernelConsts::tolerance()){

            //we plug a large error contribution if y is anything than one
            if( label != 1){
                result += 1.0;
            }
        }
        else if(std::fabs(prediction) < kernel::KernelConsts::tolerance()){

            //hval is zero. we only get contribution
            //if the label is not zero as well
            if( label > kernel::KernelConsts::tolerance()){
                result += 1.0;
            }
        }
        else{
            //do it normally
            //calculate the logarithms and check if they are
            //infinite or nana
            auto log_one_minus_h = std::log(1 - prediction);
            auto log_h = std::log(prediction);
            result += label*log_h +(1 - label)*log_one_minus_h;
        }
    }

    result *= -1;
    return result / dataset.n_examples();
}

template<typename Model, typename DatasetTp>
DynVec<real_t>
CategoricalCrossEntropy<Model, DatasetTp>::params_gradients(const DatasetTp& dataset)const{

 MSELoss<Model, DatasetTp> mse_loss(const_cast<Model&>(this->model_ref_()));
 return mse_loss.params_gradients(dataset);

}

template<typename Model, typename DatasetTp>
real_t
CategoricalCrossEntropy<Model, DatasetTp>::error_at(const row_t& p, const label_value_t& label)const{

    auto result = 0.0;
    auto prediction = this->model_ref_().value_at(p);

    //h is close to one
    if(std::fabs(1 - prediction) < kernel::KernelConsts::tolerance()){

        //we plug a large error contribution if y is anything than one
        if( label != 1){
            result = 1.0;
        }
    }
    else if(std::fabs(prediction) < kernel::KernelConsts::tolerance()){

        //hval is zero. we only get contribution
        //if the label is not zero as well
        if( label > kernel::KernelConsts::tolerance()){
            result = 1.0;
        }
    }
    else{
        //do it normally
        //calculate the logarithms and check if they are
        //infinite or nana
        auto log_one_minus_h = std::log(1 - prediction);
        auto log_h = std::log(prediction);
        result = label*log_h +(1 - label)*log_one_minus_h;
    }

    return result;

}

template<typename Model, typename DatasetTp>
DynVec<real_t>
CategoricalCrossEntropy<Model, DatasetTp>::param_gradient_at(const row_t& p, const label_value_t& label)const{

    MSELoss<Model, DatasetTp> mse_loss(const_cast<Model&>(this->model_ref_()));
    return mse_loss.param_gradient_at(p, label);

}

}

}

#endif // CATEGORICAL_CROSS_ENTROPY_H
