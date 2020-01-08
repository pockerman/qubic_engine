#ifndef PYTORCH_LINEAR_REGRESSOR_H
#define PYTORCH_LINEAR_REGRESSOR_H

#include "cubic_engine/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include <torch/torch.h>

#include <vector>

namespace cengine
{
namespace pytorch
{

/// \brief Create a regressor with hypothesis function
/// f = w_0 + w_1*x_1 +...+w_N*x_N
/// The bias term w_0 is always included so when cosntructing
/// the object this should be taken into account
class PYLinearRegressor: public torch::nn::Module
{

public:

    /// \brief The hypothesis type
    typedef torch::nn::Linear hypothesis_t;

    /// \brief The output of predict
    typedef real_t output_type;

    /// \brief Constructor.
    PYLinearRegressor(uint_t n_features);

    /// \brief Constructor
    PYLinearRegressor(const std::vector<real_t>& params);

    /// \brief Set the number of parameters for the model
    void set_model_parameters(const std::vector<real_t>& params);

    /// \brief Train the  model
    template<typename DataSetType, typename LabelsType,
             typename Trainer,typename ErrorFuncType>
    typename Trainer::output_type
    train(const DataSetType& dataset, const LabelsType& labels,
          Trainer& trainer, ErrorFuncType& func){}

    /// \brief Predict the class for the given data point
    template<typename DataPoint>
    output_type predict(const DataPoint& point)const{}

    /// \brief Returns the raw model
    const hypothesis_t& get_model()const{return hypothesis_;}

    /// \brief Return the i-th parameter
    real_t coeff(uint_t i)const{return 0.0; /*hypothesis_->weight[i].v*/;}

    /// \brief print the model
    void print(std::ostream& stream)const{hypothesis_->pretty_print(stream);}

private:

    /// \brief The hypothesis to use
    hypothesis_t hypothesis_;

};


}


}


#endif

#endif // PYTORCH_LINEAR_REGRESSOR_H
