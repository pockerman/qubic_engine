#include "cubic_engine/ml/supervised_learning/pytorch_linear_regressor.h"
#ifdef USE_PYTORCH

namespace cengine
{

namespace pytorch
{

PYLinearRegressor::PYLinearRegressor(uint_t n_features)
    :
    torch::nn::Module(),
    hypothesis_(register_module("linear", torch::nn::Linear(n_features, 1)))
{
    // always include the bias term
    hypothesis_->options.with_bias(true);
}

PYLinearRegressor::PYLinearRegressor(const std::vector<real_t>& params)
    :
    torch::nn::Module(),
    hypothesis_(register_module("linear", torch::nn::Linear(params.size(), 1)))
{

    set_model_parameters(params);

    // always include the bias term
    hypothesis_->options.with_bias(true);
}


void
PYLinearRegressor::set_model_parameters(const std::vector<real_t>& params){

    uint_t w=0;
    for(auto param : params){
        hypothesis_->weight[w++] = param;
    }
}

}
}

#endif
