#include "cubic_engine/rl/utils/torch_recurrent_generator.h"

#ifdef USE_PYTORCH

namespace cengine {
namespace rl {
namespace utils {

namespace{
torch::Tensor flatten_helper(int timesteps, int processes, torch::Tensor tensor)
{
    auto tensor_shape = tensor.sizes().vec();
    tensor_shape.erase(tensor_shape.begin());
    tensor_shape[0] = timesteps * processes;
    return tensor.view(tensor_shape);
}

}

TorchRecurrentGenerator::TorchRecurrentGenerator(int mini_batch_size, int num_envs_per_batch_,
                         torch::Tensor observations,
                         torch::Tensor hidden_states,
                         torch::Tensor actions,
                         torch::Tensor value_predictions,
                         torch::Tensor returns,
                         torch::Tensor masks,
                         torch::Tensor action_log_probs,
                         torch::Tensor advantages)
{}

bool
TorchRecurrentGenerator::done() const {
    return index_ >= indices_.size(0);
}

TorchMiniBatch
TorchRecurrentGenerator::next(){

    if (index_ >= indices_.size(0)){
            throw std::runtime_error("No minibatches left in generator.");
    }

    TorchMiniBatch mini_batch;

    // Fill minibatch with tensors of shape (timestep, process, *whatever)
    // Except hidden states, that is just (process, *whatever)
    int64_t env_index = indices_[index_].item().toLong();
    mini_batch.observations = observations_
                                  .narrow(0, 0, observations_.size(0) - 1)
                                  .narrow(1, env_index, num_envs_per_batch_);
    mini_batch.hidden_states = hidden_states_[0]
                                   .narrow(0, env_index, num_envs_per_batch_)
                                   .view({num_envs_per_batch_, -1});
    mini_batch.actions = actions_.narrow(1, env_index, num_envs_per_batch_);
    mini_batch.value_predictions = value_predictions_
                                       .narrow(0, 0, value_predictions_.size(0) - 1)
                                       .narrow(1, env_index, num_envs_per_batch_);
    mini_batch.returns = returns_.narrow(0, 0, returns_.size(0) - 1)
                             .narrow(1, env_index, num_envs_per_batch_);
    mini_batch.masks = masks_.narrow(0, 0, masks_.size(0) - 1)
                           .narrow(1, env_index, num_envs_per_batch_);
    mini_batch.action_log_probs = action_log_probs_.narrow(1, env_index,
                                                          num_envs_per_batch_);
    mini_batch.advantages = advantages_.narrow(1, env_index, num_envs_per_batch_);

    // Flatten tensors to (timestep * process, *whatever)
    int num_timesteps = mini_batch.observations.size(0);
    int num_processes = num_envs_per_batch_;

    mini_batch.observations = flatten_helper(num_timesteps, num_processes, mini_batch.observations);
    mini_batch.actions = flatten_helper(num_timesteps, num_processes, mini_batch.actions);
    mini_batch.value_predictions = flatten_helper(num_timesteps, num_processes, mini_batch.value_predictions);

    mini_batch.returns = flatten_helper(num_timesteps, num_processes, mini_batch.returns);
    mini_batch.masks = flatten_helper(num_timesteps, num_processes, mini_batch.masks);
    mini_batch.action_log_probs = flatten_helper(num_timesteps, num_processes, mini_batch.action_log_probs);
    mini_batch.advantages = flatten_helper(num_timesteps, num_processes, mini_batch.advantages);

    index_++;
    return mini_batch;

}

}
}
}

#endif
