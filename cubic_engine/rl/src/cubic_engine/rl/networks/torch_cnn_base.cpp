#include "cubic_engine/rl/networks/torch_cnn_base.h"

#ifdef USE_PYTORCH
namespace cengine{
namespace rl{
namespace nets{

TorchCNNBase::TorchCNNBase(uint_t num_inputs, uint_t hidden_size)
    :
      TorchNNBase(),
      main_(torch::nn::Conv2d(torch::nn::Conv2dOptions(num_inputs, 32, 8).stride(4)),
      torch::nn::Functional(torch::relu),
      torch::nn::Conv2d(torch::nn::Conv2dOptions(32, 64, 4).stride(2)),
      torch::nn::Functional(torch::relu),
      torch::nn::Conv2d(torch::nn::Conv2dOptions(64, 32, 3).stride(1)),
      torch::nn::Functional(torch::relu),
      torch::nn::Flatten(),
      torch::nn::Linear(32 * 7 * 7, hidden_size),
      torch::nn::Functional(torch::relu)),
      critic_linear_(torch::nn::Linear(hidden_size, 1))
{
    register_module("main", main_);
    register_module("critic_linear", critic_linear_);

    // TODO Don't hardcode this
    torch::nn::init::uniform_(main_->named_parameters()["weight"]); //, sqrt(2.), 0);
    torch::nn::init::uniform_(critic_linear_->named_parameters()["weight"]); //, 1, 0);

    train();
}

std::vector<torch::Tensor>
TorchCNNBase::forward(torch::Tensor inputs, torch::Tensor hxs,
                      torch::Tensor masks){

    // TODO don't hard code the magic constant
    auto x = main_->forward(inputs / 255.);
    return {critic_linear_->forward(x), x, hxs};
}


}
}
}
#endif
