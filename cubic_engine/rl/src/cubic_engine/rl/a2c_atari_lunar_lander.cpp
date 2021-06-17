#include "cubic_engine/rl/a2c_atari_lunar_lander.h"
#include "cubic_engine/rl/utils/torch_rollout_storage.h"
#include "cubic_engine/rl/actions/action_space.h"

namespace cengine{
namespace rl{

A2CAtariLunarLander::A2CAtariLunarLander(world_t& environment, policies::TorchPolicy& policy, const A2CAtariLunarLanderInput input)
    :
      A2C<worlds::GymLunarLanderWorld>(environment, policy, input),
      max_frames_(input.max_frames),
      batch_size_(input.batch_size)
{}


void
A2CAtariLunarLander::train(){


    auto num_updates = max_frames_ / (batch_size_ * this->num_envs());

    auto observation_space_shape = this->world()->observation_space_shape();
    auto action_space_type = this->world()->action_space_type();
    auto action_space_shape = this->world()->action_space_shape();

    actions::ActionSpace space{action_space_type, action_space_shape};

    utils::TorchRolloutStorage storage(batch_size_, this->num_envs(),
                                       observation_space_shape,
                                       space, hidden_size_, this->device());

    for (auto update = 0; update < num_updates; ++update)
    {
        for (int step = 0; step < batch_size_; ++step)
        {

            std::vector<torch::Tensor> act_result;
            {
               torch::NoGradGuard no_grad;
               act_result = this->policy()->act(storage.get_observations()[step],
                                                storage.get_hidden_states()[step],
                                                storage.get_masks()[step]);
            }

            auto actions_tensor = act_result[1].cpu().to(torch::kFloat);
            real_t *actions_array = actions_tensor.data_ptr<real_t>();
            std::vector<std::vector<real_t>> actions(this->num_envs());

            // loop over the environments and update the actions
            for (auto i = 0; i < this->num_envs(); ++i)
            {
                if (space.type == "Discrete")
                {
                    actions[i] = {actions_array[i]};
                }
                else
                {
                    for (auto j = 0; j < action_space_shape[0]; j++)
                    {
                        actions[i].push_back(actions_array[i * action_space_shape[0] + j]);
                    }
                }
            }

            auto step_request = this->world()->make_step_request(); //std::make_shared<StepParam>();
            step_request->actions = actions;
            step_request->render = render;

            this->world()->step_from_request(step_request)

            //Request<StepParam> step_request("step", step_param);
            //communicator.send_request(step_request);

            std::vector<float> rewards;
            std::vector<float> real_rewards;
            std::vector<std::vector<bool>> dones_vec;

        }
    }

}

}
}
