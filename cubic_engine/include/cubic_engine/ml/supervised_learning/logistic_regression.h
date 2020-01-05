/* 
 * File:   linear_logistic_regression.h
 * Author: david
 *
 * Created on June 20, 2016, 11:56 AM
 */

#ifndef LOGISTIC_REGRESSION_H
#define	LOGISTIC_REGRESSION_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/base/types.h"

#include <boost/noncopyable.hpp>
#include <vector>
namespace cengine
{
    
/// \brief LogisticRegression. Logistic Regression classifier
template<typename HypothesisType, typename TransformerType>
class LogisticRegression: private boost::noncopyable
{

 public:

    typedef HypothesisType hypothesis_t;
    typedef TransformerType transformer_t;
    typedef uint_t output_t;

    /// \brief Constructor
    LogisticRegression();

    /// \brief Constructor. Set the initial coeffs of the
    /// underlying hypothesis model
    LogisticRegression(const std::vector<real_t>& coeffs);

    /// \brief Set the number of parameters for the model
    void set_model_parameters(const std::vector<real_t>& coeffs);

    /// \brief Train the model
    template<typename DataSetType, typename LabelsType,
             typename Trainer, typename ErrorFuncType>
    typename Trainer::output_type
    train(const DataSetType& dataset, const LabelsType& labels,
          Trainer& trainer, ErrorFuncType& func);

    /// \brief Predict the class for the given data point
    template<typename DataPoint>
    output_t predict(const DataPoint& point)const;

    /// \brief Returns the raw model
    const hypothesis_t& get_model()const{return hypothesis_;}

    /// \brief Return the i-th parameter
    real_t coeff(uint_t i)const{return hypothesis_.coeff(i);}

private:

    hypothesis_t hypothesis_;
 };
 
template<typename HypothesisType, typename TransformerType>
LogisticRegression<HypothesisType, TransformerType>::LogisticRegression()
:
hypothesis_()
{}

template<typename HypothesisType, typename TransformerType>
LogisticRegression<HypothesisType, TransformerType>::LogisticRegression(const std::vector<real_t>& coeffs)
    :
      hypothesis_(coeffs)
{}

template<typename HypothesisType, typename TransformerType>
template<typename DataSetType, typename LabelsType,
         typename Trainer, typename ErrorFuncType>
typename Trainer::output_type
LogisticRegression<HypothesisType, TransformerType>::train(const DataSetType& dataset, const LabelsType& labels,
                                                           Trainer& trainer, ErrorFuncType& func){
    
    return trainer.solve(dataset, labels, func, hypothesis_);
    
}

template<typename HypothesisType, typename TransformerType>
void 
LogisticRegression<HypothesisType, TransformerType>::set_model_parameters(const std::vector<real_t>& coeffs){
    
    hypothesis_.set_coeffs(coeffs);
}

template<typename HypothesisType, typename TransformerType>
template<typename DataPoint>
typename LogisticRegression<HypothesisType, TransformerType>::output_t
LogisticRegression<HypothesisType, TransformerType>::predict(const DataPoint& point)const{
    
    TransformerType transformer(hypothesis_);
    auto value = transformer.value(point);

    if(value >= 0.5){
        return 1;
    }

    return 0;
}
    
}

#endif

