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
class PYT_LinearRegressor: public torch::nn::Module
{

public:

    /// \brief The hypothesis type
    typedef torch::nn::Linear hypothesis_t;

    /// \brief The output of predict
    typedef real_t output_t;

    /// \brief Constructor.
    PYT_LinearRegressor(uint_t n_features);

    /// \brief Constructor
    PYT_LinearRegressor(const std::vector<real_t>& params);

    /// \brief Set the number of parameters for the model
    void set_model_parameters(const std::vector<real_t>& params);

    /// \brief Train the  model
    template<typename DataSetType, typename LabelsType, typename Trainer>
    typename Trainer::output_t
    train(const DataSetType& dataset, const LabelsType& labels, Trainer& trainer);

    /// \brief Predict the class for the given data point
    template<typename DataPoint>
    output_t predict(const DataPoint& point)const{}

    /// \brief Returns the raw model
    const hypothesis_t& get_model()const{return hypothesis_;}

    /// \brief Returns the raw model
    hypothesis_t& get_model(){return hypothesis_;}

    /// \brief Return the i-th parameter
    real_t coeff(uint_t i)const{return 0.0; /*hypothesis_->weight[i].v*/;}

    /// \brief print the model
    void print(std::ostream& stream)const{hypothesis_->pretty_print(stream);}

private:

    /// \brief The hypothesis to use
    hypothesis_t hypothesis_;

};


template<typename DataSetType, typename LabelsType, typename Trainer>
typename Trainer::output_t
PYT_LinearRegressor::train(const DataSetType& dataset, const LabelsType& labels, Trainer& trainer){

    return trainer.solve(dataset, labels, hypothesis_);
}


}


}


#endif

#endif // PYTORCH_LINEAR_REGRESSOR_H
