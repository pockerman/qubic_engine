#include "cubic_engine/rl/utils/torch_forward_generator.h"

#ifdef USE_PYTORCH
namespace cengine {
namespace rl {
namespace utils {

TorchFeedForwardGenerator::TorchFeedForwardGenerator(int mini_batch_size,
                         torch::Tensor observations,
                         torch::Tensor hidden_states,
                         torch::Tensor actions,
                         torch::Tensor value_predictions,
                         torch::Tensor returns,
                         torch::Tensor masks,
                         torch::Tensor action_log_probs,
                         torch::Tensor advantages)
    :
      index_(0),
      observations_(observations),
      hidden_states_(hidden_states),
      actions_(actions),
      value_predictions_(value_predictions),
      returns_(returns),
      masks_(masks),
      action_log_probs_(action_log_probs),
      advantages_(advantages),
      indices_()
{

    int batch_size = advantages.numel();
    indices_ = torch::randperm(batch_size, torch::TensorOptions(torch::kLong)).view({-1, mini_batch_size});
}

bool TorchFeedForwardGenerator::done() const
{
    return index_ >= indices_.size(0);
}

TorchMiniBatch
TorchFeedForwardGenerator::next()
{
    if (index_ >= indices_.size(0))
    {
        throw std::runtime_error("No minibatches left in generator.");
    }

    TorchMiniBatch mini_batch;

    int timesteps = observations_.size(0) - 1;

    auto observations_shape = observations_.sizes().vec();
    observations_shape.erase(observations_shape.begin());
    observations_shape[0] = -1;

    mini_batch.observations = observations_.narrow(0, 0, timesteps).view(observations_shape).index(indices_[index_]);
    mini_batch.hidden_states = hidden_states_.narrow(0, 0, timesteps).view({-1, hidden_states_.size(-1)}).index(indices_[index_]);
    mini_batch.actions = actions_.view({-1, actions_.size(-1)}).index(indices_[index_]);
    mini_batch.value_predictions = value_predictions_.narrow(0, 0, timesteps).view({-1, 1}).index(indices_[index_]);
    mini_batch.returns = returns_.narrow(0, 0, timesteps).view({-1, 1}).index(indices_[index_]);
    mini_batch.masks = masks_.narrow(0, 0, timesteps).view({-1, 1}).index(indices_[index_]);
    mini_batch.action_log_probs = action_log_probs_.view({-1, 1}).index(indices_[index_]);
    mini_batch.advantages = advantages_.view({-1, 1}).index(indices_[index_]);

    index_++;
    return mini_batch;
}


}
}
}
#endif
