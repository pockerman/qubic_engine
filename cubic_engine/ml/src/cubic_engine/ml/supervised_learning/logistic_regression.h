#ifndef LOGISTIC_REGRESSION_H
#define	LOGISTIC_REGRESSION_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/maths/matrix_utilities.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/maths/functions/sigmoid_function.h"

#include <boost/noncopyable.hpp>
#include <vector>
#include <ostream>


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

    ///
    /// \brief Constructor
    ///
    LogisticRegression();

    ///
    /// \brief Constructor. Set the initial coeffs of the
    /// underlying hypothesis model
    ///
    LogisticRegression(const std::vector<real_t>& coeffs);

    ///
    /// \brief Set the number of parameters for the model
    ///
    void set_model_parameters(const std::vector<real_t>& coeffs);

    ///
    /// \brief Train the model
    ///
    template<typename DataSetType, typename LabelsType, typename Trainer>
    typename Trainer::output_t
    train(const DataSetType& dataset, const LabelsType& labels, Trainer& trainer);

    ///
    /// \brief Train the model
    ///
    template<typename DataSetType, typename LabelsType,
             typename Trainer, typename RegularizerType>
    typename Trainer::output_t
    train(const DataSetType& dataset, const LabelsType& labels,
          Trainer& trainer, const RegularizerType& regularizer);

    ///
    /// \brief Predict the class for the given data point
    ///
    template<typename DataPoint>
    output_t predict(const DataPoint& point)const;

    ///
    /// \brief Predict on the data set
    ///
    template<typename DataSet, typename OutputType>
    void predict(const DataSet& point, OutputType& out)const;

    ///
    /// \brief Returns the raw model
    ///
    const hypothesis_t& get_model()const{return hypothesis_;}

    ///
    /// \brief Return the i-th parameter
    ///
    real_t coeff(uint_t i)const{return hypothesis_.coeff(i);}

    ///
    /// \brief Print the model coeffs
    ///
    std::ostream& print(std::ostream& out)const;

private:

    hypothesis_t hypothesis_;

    TransformerType transformer_;
 };
 
template<typename HypothesisType, typename TransformerType>
LogisticRegression<HypothesisType, TransformerType>::LogisticRegression()
:
hypothesis_(),
transformer_(hypothesis_)
{}

template<typename HypothesisType, typename TransformerType>
LogisticRegression<HypothesisType, TransformerType>::LogisticRegression(const std::vector<real_t>& coeffs)
    :
      hypothesis_(coeffs),
      transformer_(hypothesis_)
{}

template<typename HypothesisType, typename TransformerType>
template<typename DataSetType, typename LabelsType, typename Trainer>
typename Trainer::output_t
LogisticRegression<HypothesisType, TransformerType>::train(const DataSetType& dataset, const LabelsType& labels, Trainer& trainer ){
    
    kernel::MSEFunction<TransformerType, DataSetType, LabelsType> error(transformer_);
    return trainer.solve(dataset, labels, error);
}

template<typename HypothesisType, typename TransformerType>
template<typename DataSetType, typename LabelsType,
         typename Trainer, typename RegularizerType>
typename Trainer::output_t
LogisticRegression<HypothesisType, TransformerType>::train(const DataSetType& dataset, const LabelsType& labels,
                                                           Trainer& trainer, const RegularizerType& regularizer){

    kernel::MSEFunction<TransformerType, DataSetType, LabelsType, RegularizerType> error(transformer_, regularizer);
    return trainer.solve(dataset, labels, error);
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
    
    //TransformerType transformer(hypothesis_);
    auto value = transformer_.value(point);

    if(value >= 0.5){
        return 1;
    }

    return 0;
}

template<typename HypothesisType, typename TransformerType>
template<typename DataSet, typename OutputType>
void
LogisticRegression<HypothesisType, TransformerType>::predict(const DataSet& data, OutputType& out)const{


    if(out.size() != data.rows()){
        throw std::logic_error("out vector size " +
                               std::to_string(out.size()) +
                               " not equal to " +
                               std::to_string(data.rows()));
    }

    for(uint_t r = 0; r < data.rows(); ++r){

        auto row = kernel::get_row(data, r);
        out[r] = predict(row);
    }
}

template<typename HypothesisType, typename TransformerType>
std::ostream&
LogisticRegression<HypothesisType, TransformerType>::print(std::ostream& out)const{

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

template<typename HypothesisType, typename TransformerType>
inline
std::ostream& operator<<(std::ostream& out,
                         const LogisticRegression<HypothesisType, TransformerType>& classifier){
    return classifier.print(out);
}
    
}

#endif

