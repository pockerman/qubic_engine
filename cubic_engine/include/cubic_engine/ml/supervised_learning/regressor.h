#ifndef REGRESSOR_H
#define	REGRESSOR_H

#include "cubic_engine/base/config.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/functions/real_vector_polynomial.h"
#include <boost/noncopyable.hpp>
#include <vector>


namespace cengine
{
    
/// \brief Regressor class.
template<typename HypothesisType>
class Regressor: private boost::noncopyable
{
    
public:

    typedef HypothesisType hypothesis_t;

    /// \brief The output of predict
    typedef real_t output_type;

     /// \brief Constructor
     Regressor();

     /// \brief Constructor
     Regressor(const std::vector<real_t>& params);

     /// \brief Set the number of parameters for the model
     void set_model_parameters(const std::vector<real_t>& params);

     /// \brief Train the  model
     template<typename DataSetType, typename LabelsType,
              typename Trainer,typename ErrorFuncType>
     typename Trainer::output_type
     train(const DataSetType& dataset, const LabelsType& labels,
           Trainer& trainer,const ErrorFuncType& func);

     /// \brief Predict the class for the given data point
     template<typename DataPoint>
     output_type predict(const DataPoint& point)const;

     /// \brief Returns the raw model
     const hypothesis_t& get_model()const{return hypothesis_;}

     /// \brief Return the i-th parameter
     real_t coeff(uint_t i)const{return hypothesis_.coeff(i);}

private:

     hypothesis_t hypothesis_;
};

template<typename HypothesisType>
Regressor<HypothesisType>::Regressor()
    :
      hypothesis_()
{}

template<typename HypothesisType>
Regressor<HypothesisType>::Regressor(const std::vector<real_t>& params)
    :
      hypothesis_(params)
{}

template<typename HypothesisType>
template<typename DataSetType, typename LabelsType,
         typename Trainer,typename ErrorFuncType>
typename Trainer::output_type
Regressor<HypothesisType>::train(const DataSetType& dataset, const LabelsType& labels,
                                 Trainer& trainer, const ErrorFuncType& func){
    
    return trainer.solve(dataset, labels, func, hypothesis_);
}

template<typename HypothesisType>
void 
Regressor<HypothesisType>::set_model_parameters(const std::vector<real_t>& params){
    
    hypothesis_.set_coeffs(params);
}

template<typename HypothesisType>
template<typename DataPoint>
typename Regressor<HypothesisType>::output_type
Regressor<HypothesisType>::predict(const DataPoint& point)const{
    
   return hypothesis_.value(point);
}


/// \brief linear regression
using LinearRegression = Regressor<kernel::RealVectorPolynomialFunction>;
    
}



#endif	/* LINEAR_REGRESSION_H */

