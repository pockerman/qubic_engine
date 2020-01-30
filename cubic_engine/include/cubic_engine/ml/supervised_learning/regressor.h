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
    typedef real_t output_t;

     /// \brief Constructor
     Regressor();

     /// \brief Constructor
     Regressor(const std::vector<real_t>& params);

     /// \brief Set the number of parameters for the model
     void set_model_parameters(const std::vector<real_t>& params);

     /// \brief Train the  model
     template<typename DataSetType, typename LabelsType, typename Trainer>
     typename Trainer::output_t
     train(const DataSetType& dataset, const LabelsType& labels, Trainer& trainer);

     /// \brief Train the  model using a regularizer
     template<typename DataSetType, typename LabelsType,
              typename Trainer, typename RegularizerType>
     typename Trainer::output_t
     train(const DataSetType& dataset, const LabelsType& labels,
           Trainer& trainer, const  RegularizerType& regularizer);

     /// \brief Predict the class for the given data point
     template<typename DataPoint>
     output_t predict(const DataPoint& point)const;

     /// \brief Returns the raw model
     const hypothesis_t& get_model()const{return hypothesis_;}

     /// \brief Return the i-th parameter
     real_t coeff(uint_t i)const{return hypothesis_.coeff(i);}

     /// \brief Print the model coeffs
     std::ostream& print(std::ostream& out)const;

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
template<typename DataSetType, typename LabelsType, typename Trainer>
typename Trainer::output_t
Regressor<HypothesisType>::train(const DataSetType& dataset, const LabelsType& labels, Trainer& trainer){
    
    return trainer.solve(dataset, labels, hypothesis_);
}

template<typename HypothesisType>
template<typename DataSetType, typename LabelsType,
         typename Trainer, typename RegularizerType>
typename Trainer::output_t
Regressor<HypothesisType>::train(const DataSetType& dataset, const LabelsType& labels,
                                 Trainer& trainer, const  RegularizerType& regularizer){

    return trainer.solve(dataset, labels, hypothesis_, regularizer);
}

template<typename HypothesisType>
void 
Regressor<HypothesisType>::set_model_parameters(const std::vector<real_t>& params){
    
    hypothesis_.set_coeffs(params);
}

template<typename HypothesisType>
template<typename DataPoint>
typename Regressor<HypothesisType>::output_t
Regressor<HypothesisType>::predict(const DataPoint& point)const{
    
   return hypothesis_.value(point);
}

template<typename HypothesisType>
std::ostream&
Regressor<HypothesisType>::print(std::ostream& out)const{

    for(uint_t c=0; c < hypothesis_.n_coeffs(); ++c){
        out<<hypothesis_.coeff(c);

        if(c == hypothesis_.n_coeffs() - 1){
            out<<"\n";
        }
        else{
            out<<",";
        }
    }

    return out;
}

template<typename HypothesisType>
inline
std::ostream& operator<<(std::ostream& out, const Regressor<HypothesisType>& regressor){
    return regressor.print(out);
}


/// \brief linear regression
using LinearRegression = Regressor<kernel::RealVectorPolynomialFunction>;
    
}



#endif	/* LINEAR_REGRESSION_H */

