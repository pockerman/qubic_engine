#ifndef TORCH_ROLLOUT_STORAGE_H
#define TORCH_ROLLOUT_STORAGE_H

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

    ///
    /// \brief RolloutStorage
    ///
    RolloutStorage(int64_t num_steps,
                       int64_t num_processes,
                       c10::ArrayRef<int64_t> obs_shape,
                       actions::ActionSpace action_space,
                       int64_t hidden_state_size,
                       torch::Device device);

    ///
    /// \brief RolloutStorage
    ///
    RolloutStorage(std::vector<RolloutStorage *> individual_storages, torch::Device device);

    ///
    /// \brief after_update
    ///
    void after_update();

    ///
    /// \brief compute_returns
    ///
    void compute_returns(torch::Tensor next_value, bool use_gae,
                         float gamma, float tau);

    ///
    /// \brief feed_forward_generator
    ///
    std::unique_ptr<TorchGeneratorBase>
    feed_forward_generator(torch::Tensor advantages, int num_mini_batch);

    ///
    /// \brief insert
    ///
    void insert(torch::Tensor observation,
                torch::Tensor hidden_state,
                torch::Tensor action,
                torch::Tensor action_log_prob,
                torch::Tensor value_prediction,
                torch::Tensor reward,
                torch::Tensor mask);


    ///
    /// \brief recurrent_generator
    /// \param advantages
    /// \param num_mini_batch
    /// \return
    ///
    std::unique_ptr<TorchGeneratorBase>
    recurrent_generator(torch::Tensor advantages, int num_mini_batch);

    ///
    /// \brief set_first_observation
    /// \param observation
    ///
    void set_first_observation(torch::Tensor observation);

    ///
    /// \brief to
    /// \param device
    ///
    void to(torch::Device device);

    ///
    /// \brief get_actions
    /// \return
    ///
    const torch::Tensor& get_actions() const { return actions_; }

    ///
    /// \brief get_action_log_probs
    /// \return
    ///
    const torch::Tensor& get_action_log_probs() const { return action_log_probs_; }

    ///
    /// \brief get_hidden_states
    /// \return
    ///
    const torch::Tensor& get_hidden_states() const { return hidden_states_; }

    ///
    /// \brief get_masks
    /// \return
    ///
    const torch::Tensor& get_masks() const { return masks_; }

    ///
    /// \brief get_observations
    /// \return
    ///
    const torch::Tensor& get_observations() const { return observations_; }

    ///
    /// \brief get_returns
    /// \return
    ///
    const torch::Tensor& get_returns() const { return returns_; }

    ///
    /// \brief get_rewards
    /// \return
    ///
    const torch::Tensor& get_rewards() const { return rewards_; }

    ///
    /// \brief get_value_predictions
    /// \return
    ///
    const torch::Tensor& get_value_predictions() const{return value_predictions_;}

    ///
    /// \brief set_actions
    /// \param actions
    ///
    void set_actions(torch::Tensor actions) { actions_ = actions; }

    ///
    /// \brief set_action_log_probs
    /// \param action_log_probs
    ///
    void set_action_log_probs(torch::Tensor action_log_probs) { action_log_probs_ = action_log_probs; }

    ///
    /// \brief set_hidden_states
    /// \param hidden_states
    ///
    void set_hidden_states(torch::Tensor hidden_states) { hidden_states_ = hidden_states; }

    ///
    /// \brief set_masks
    /// \param masks
    ///
    void set_masks(torch::Tensor masks) { masks_ = masks; }

    ///
    /// \brief set_observations
    /// \param observations
    ///
    void set_observations(torch::Tensor observations) {observations_ = observations; }

    ///
    /// \brief set_returns
    /// \param returns
    ///
    void set_returns(torch::Tensor returns) { returns_ = returns; }

    ///
    /// \brief set_rewards
    /// \param rewards
    ///
    void set_rewards(torch::Tensor rewards) { rewards_ = rewards; }

    ///
    /// \brief set_value_predictions
    /// \param value_predictions
    ///
    void set_value_predictions(torch::Tensor value_predictions){value_predictions_ = value_predictions;}

private:

    int64_t num_steps_;
    int64_t step_;
    torch::Tensor observations_;
    torch::Tensor hidden_states_;
    torch::Tensor rewards_;
    torch::Tensor value_predictions_;
    torch::Tensor returns_;
    torch::Tensor action_log_probs_;
    torch::Tensor actions_;
    torch::Tensor masks_;
    torch::Device device_;


};

using TorchRolloutStorage = RolloutStorage<torch_t>;

}

}
}
#endif
#endif // ROLLOUT_STORAGE_H
