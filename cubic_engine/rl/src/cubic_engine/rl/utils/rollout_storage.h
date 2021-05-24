#ifndef ROLLOUT_STORAGE_H
#define ROLLOUT_STORAGE_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/actions/action_space.h"
#include "cubic_engine/rl/utils/torch_generator_base.h"
#include "torch/torch.h"

#include <memory>
#include <vector>

namespace cengine{
namespace rl {
namespace utils {

template<typename StorageT> class RolloutStorage;

template<>
class RolloutStorage<torch_t>
{

public:

    RolloutStorage(int64_t num_steps,
                       int64_t num_processes,
                       c10::ArrayRef<int64_t> obs_shape,
                       actions::ActionSpace action_space,
                       int64_t hidden_state_size,
                       torch::Device device);

    RolloutStorage(std::vector<RolloutStorage *> individual_storages, torch::Device device);

    void after_update();
    void compute_returns(torch::Tensor next_value,
                         bool use_gae,
                         float gamma,
                         float tau);

    std::unique_ptr<TorchGeneratorBase> feed_forward_generator(torch::Tensor advantages,
                                                      int num_mini_batch);
    void insert(torch::Tensor observation,
                torch::Tensor hidden_state,
                torch::Tensor action,
                torch::Tensor action_log_prob,
                torch::Tensor value_prediction,
                torch::Tensor reward,
                torch::Tensor mask);


    std::unique_ptr<TorchGeneratorBase> recurrent_generator(torch::Tensor advantages,
                                                   int num_mini_batch);

    void set_first_observation(torch::Tensor observation);
    void to(torch::Device device);

    const torch::Tensor &get_actions() const { return actions; }
    const torch::Tensor &get_action_log_probs() const { return action_log_probs; }
    const torch::Tensor &get_hidden_states() const { return hidden_states; }
    const torch::Tensor &get_masks() const { return masks; }
    const torch::Tensor &get_observations() const { return observations; }
    const torch::Tensor &get_returns() const { return returns; }
    const torch::Tensor &get_rewards() const { return rewards; }
    const torch::Tensor &get_value_predictions() const{return value_predictions;}
    void set_actions(torch::Tensor actions) { this->actions = actions; }
    void set_action_log_probs(torch::Tensor action_log_probs) { this->action_log_probs = action_log_probs; }
    void set_hidden_states(torch::Tensor hidden_states) { this->hidden_states = hidden_states; }
    void set_masks(torch::Tensor masks) { this->masks = masks; }
    void set_observations(torch::Tensor observations) { this->observations = observations; }
    void set_returns(torch::Tensor returns) { this->returns = returns; }
    void set_rewards(torch::Tensor rewards) { this->rewards = rewards; }
    void set_value_predictions(torch::Tensor value_predictions){this->value_predictions = value_predictions;}

private:

    torch::Tensor observations;
    torch::Tensor hidden_states;
    torch::Tensor rewards;
    torch::Tensor value_predictions;
    torch::Tensor returns;
    torch::Tensor action_log_probs;
    torch::Tensor actions;
    torch::Tensor masks;
    torch::Device device;
    int64_t num_steps;
    int64_t step;

};

using TorchRolloutStorage = RolloutStorage<torch_t>;

}

}
}
#endif
#endif // ROLLOUT_STORAGE_H
