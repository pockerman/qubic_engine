#include "cubic_engine/rl/a2c_atari_lunar_lander.h"
#include "cubic_engine/rl/utils/torch_rollout_storage.h"
#include "cubic_engine/rl/actions/action_space.h"

namespace cengine{
namespace rl{

template<typename T>
std::vector<T> flatten_vector(std::vector<T> const &input)
{
    return input;
}

template <typename T>
std::vector<real_t> flatten_vector(std::vector<std::vector<T>> const &input)
{
    std::vector<real_t> output;

    for (auto const &element : input)
    {
        auto sub_vector = flatten_vector(element);

        output.reserve(output.size() + sub_vector.size());
        output.insert(output.end(), sub_vector.cbegin(), sub_vector.cend());
    }

    return output;
}

A2CAtariLunarLander::A2CAtariLunarLander(world_t& environment, policies::TorchPolicy& policy, const A2CAtariLunarLanderInput input)
    :
      A2C<worlds::GymLunarLanderWorld>(environment, policy, input),
      max_frames_(input.max_frames),
      batch_size_(input.batch_size)
{}


void
A2CAtariLunarLander::train(){


    auto num_updates = max_frames_ / (batch_size_ * this->num_envs());

    auto observation_space_shape = this->world()->observation_space_shape_as_torch();
    auto observation_shape = observation_space_shape;
    observation_shape.insert(observation_shape.begin(), this->num_envs());

    auto action_space_type = this->world()->action_space_type();
    auto action_space_shape = this->world()->action_space_shape();


    torch::Tensor observation;
    std::vector<real_t> observation_vec;

    if (observation_space_shape.size() > 1){
        //observation_vec = flatten_vector(communicator.get_response<CnnResetResponse>()->observation);
        observation_vec = flatten_vector(this->world()->reset());
        //observation = torch::from_blob(observation_vec.data(), observation_shape).to(device);
    }
    else{
        //observation_vec = flatten_vector(communicator.get_response<MlpResetResponse>()->observation);
        //observation = torch::from_blob(observation_vec.data(), observation_shape).to(device);
   }


    actions::ActionSpace space{action_space_type, action_space_shape};

    // set up the policy correctly

    utils::TorchRolloutStorage storage(batch_size_, this->num_envs(),
                                       observation_space_shape,
                                       space, hidden_size_, this->device());

    storage.set_first_observation(observation);

    auto episode_count = 0;
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

            auto [observations, rewards, done, any] = this->world()->vector_step(actions); //make_step_request(); //std::make_shared<StepParam>();
            //step_request->actions = actions;

            // render
            //step_request->render = render;

            // step in the environment and return
            // a TimeStep
            //auto [observations, rewards, done, any] = this->world()->step_from_request(step_request);

            //Request<StepParam> step_request("step", step_param);
            //communicator.send_request(step_request);
            std::vector<float> real_rewards;
            std::vector<std::vector<bool>> dones_vec;


            if (observation_space_shape.size() > 1)
            {

                //auto step_result = communicator.get_response<CnnStepResponse>();
                auto observation_vec = flatten_vector(observations);

                auto observation = torch::from_blob(observation_vec.data(), observation_shape).to(this->device());
                /*auto raw_reward_vec = flatten_vector(step_result->real_reward);

                auto reward_tensor = torch::from_blob(raw_reward_vec.data(), {num_envs}, torch::kFloat);
                returns = returns * discount_factor + reward_tensor;
                returns_rms->update(returns);
                reward_tensor = torch::clamp(reward_tensor / torch::sqrt(returns_rms->get_variance() + 1e-8),
                                             -reward_clip_value, reward_clip_value);

                rewards = std::vector<float>(reward_tensor.data_ptr<float>(),
                                             reward_tensor.data_ptr<float>() + reward_tensor.numel());
                real_rewards = flatten_vector(step_result->real_reward);
                dones_vec = step_result->done;*/
            }
            else
            {
                //auto step_result = communicator.get_response<MlpStepResponse>();
                /*observation_vec = flatten_vector(step_result->observation);
                observation = torch::from_blob(observation_vec.data(), observation_shape).to(device);
                auto raw_reward_vec = flatten_vector(step_result->real_reward);
                auto reward_tensor = torch::from_blob(raw_reward_vec.data(), {num_envs}, torch::kFloat);
                returns = returns * discount_factor + reward_tensor;
                returns_rms->update(returns);
                reward_tensor = torch::clamp(reward_tensor / torch::sqrt(returns_rms->get_variance() + 1e-8),
                                             -reward_clip_value, reward_clip_value);
                rewards = std::vector<float>(reward_tensor.data_ptr<float>(), reward_tensor.data_ptr<float>() + reward_tensor.numel());
                real_rewards = flatten_vector(step_result->real_reward);
                dones_vec = step_result->done;*/
            }

            for (auto i = 0; i < this->num_envs(); ++i){

                  /*running_rewards[i] += real_rewards[i];
                  if (dones_vec[i][0])
                  {
                      reward_history[episode_count % reward_average_window_size] = running_rewards[i];
                      running_rewards[i] = 0;
                      returns[i] = 0;
                      episode_count++;
                  }*/
             }

            auto dones = torch::zeros({static_cast<long>(this->num_envs()), 1},
                                      c10::TensorOptions(this->device()));

            for (int i = 0; i < this->num_envs(); ++i)
            {
                dones[i][0] = static_cast<int>(dones_vec[i][0]);
            }

            storage.insert(observation, act_result[3],
                            act_result[1], act_result[2],
                            act_result[0], torch::from_blob(rewards.data(), {static_cast<torch_int_t>(this->num_envs()), 1}).to(this->device()),
                            1 - dones);

        }// for batch size


        torch::Tensor next_value;
        {
            torch::NoGradGuard no_grad;
            next_value = this->policy()->get_values(
                                   storage.get_observations()[-1],
                                   storage.get_hidden_states()[-1],
                                   storage.get_masks()[-1])
                             .detach();
        }

        storage.compute_returns(next_value, this->use_gae(), this->gamma(), this->gae());

        auto decay_level = this->get_decay_level(update, num_updates);

        auto update_data = this->update(storage, decay_level);
        storage.after_update();

        /// TODO: Fix the logging
        /*if (update % log_interval == 0 && update > 0)
        {
            auto total_steps = (update + 1) * batch_size * num_envs;
            auto run_time = std::chrono::high_resolution_clock::now() - start_time;
            auto run_time_secs = std::chrono::duration_cast<std::chrono::seconds>(run_time);
            auto fps = total_steps / (run_time_secs.count() + 1e-9);
            spdlog::info("---");
            spdlog::info("Update: {}/{}", update, num_updates);
            spdlog::info("Total frames: {}", total_steps);
            spdlog::info("FPS: {}", fps);
            for (const auto &datum : update_data)
            {
                spdlog::info("{}: {}", datum.name, datum.value);
            }
            float average_reward = std::accumulate(reward_history.begin(), reward_history.end(), 0);
            average_reward /= episode_count < reward_average_window_size ? episode_count : reward_average_window_size;
            spdlog::info("Reward: {}", average_reward);
            render = average_reward >= render_reward_threshold;
        }*/
    }

}

}
}
