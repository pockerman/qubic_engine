#include "cubic_engine/ml/neural_network/torch_output_layers.h"

#ifdef USE_PYTORCH

namespace cengine {
namespace ml {
namespace nets {

LinearOutputLayer::LinearOutputLayer(uint_t num_inputs, uint_t num_outputs)
    :
      linear(num_inputs, num_outputs)
{
    register_module("linear", linear);
}

BernoulliOutput::BernoulliOutput(uint_t num_inputs, uint_t num_outputs)
    :
    LinearOutputLayer(num_inputs, num_outputs)
{
    init_weights(linear->named_parameters(), 0.01, 0);
}

std::unique_ptr<Distribution>
BernoulliOutput::forward(torch::Tensor x){

    x = linear(x);
    return std::make_unique<Bernoulli>(nullptr, &x);

}

CategoricalOutput::CategoricalOutput(uint_t num_inputs, uint_t num_outputs)
    :
     LinearOutputLayer(num_inputs, num_outputs)
{
    init_weights(linear->named_parameters(), 0.01, 0);
}


std::unique_ptr<Distribution>
CategoricalOutput::forward(torch::Tensor x){

    x = linear(x);
    return std::make_unique<Categorical>(nullptr, &x);

}

NormalOutput::NormalOutput(uint_t num_inputs, uint_t num_outputs)
    :
     LinearOutputLayer(num_inputs, num_outputs),
     scale_log()
{
    scale_log = register_parameter("scale_log", torch::zeros({num_outputs}));
    init_weights(linear_loc->named_parameters(), 1, 0);
}


std::unique_ptr<Distribution>
NormalOutput::forward(torch::Tensor x){

    auto loc = linear(x);
    auto scale = scale_log.exp();
    return std::make_unique<Normal>(loc, scale);

}

}
}
}
#endif
