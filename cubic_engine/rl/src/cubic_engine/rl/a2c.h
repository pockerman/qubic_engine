#ifndef A2C_H
#define A2C_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/utils/torch_rollout_storage.h"
#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/policies/torch_policy.h"
#include "cubic_engine/rl/utils/update_datum.h"

#include "torch/torch.h"
#include <memory>
#include <vector>

namespace cengine {
namespace rl {

namespace policies {
/// Forward declarations
class TorchPolicy;
}



namespace utils {

//class RolloutStorage;
template<typename T>
struct UpdateDatum;
}

///
/// \brief The A2CInput struct
///
struct A2CInput
{
    typedef real_t value_t ;


    value_t actor_loss_coef;
    value_t value_loss_coef;
    value_t entropy_coef;
    value_t learning_rate;
    value_t original_learning_rate;
    value_t epsilon{1e-8};
    value_t gamma{0.99};
    value_t max_grad_norm{0.5};

    ///
    /// \brief The GAE factor
    ///
    real_t gae{0.9};

    ///
    /// \brief
    ///
    bool use_gae{false};

    ///
    /// \brief
    ///
    bool use_decay_lr{false};

    ///
    /// \brief num_envs
    ///
    uint_t num_envs;

    ///
    /// \brief device
    ///
    torch::Device device;

};

///
/// \brief The A2C class. PyTorch based implementation of A2C algorithm
///
template<typename WorldTp>
class A2C
{

  public:

    ///
    /// \brief value_t
    ///
    typedef real_t value_t ;

    ///
    /// \brief The type of the world
    ///
    typedef WorldTp world_t;

    ///
    /// \brief state_t
    ///
    typedef typename WorldTp::state_t state_t;

    ///
    /// \brief Destructor
    ///
    virtual ~A2C() = default;

    ///
    /// \brief update
    ///
    std::vector<utils::UpdateDatum<value_t>>
    update(utils::TorchRolloutStorage &rollouts, value_t decay_level = 1);

    ///
    /// \brief decay_learning_rate
    ///
    void decay_learning_rate(value_t decay_level=1.0);

    ///
    /// \brief train
    ///
    virtual void train() = 0;

    ///
    /// \brief num_envs. Returns the number of environments
    ///
    uint_t num_envs()const{return input_.num_envs;}

    ///
    /// \brief use_gae
    ///
    bool use_gae()const{return input_.use_gae;}

    ///
    /// \brief gamma Returns the discount factor
    ///
    real_t gamma()const{return input_.gamma;}

    ///
    /// \brief gae. Returns the gae factor
    ///
    real_t gae()const{return input_.gae;}

    ///
    /// \brief device
    ///
    torch::Device device()const{return input_.device;}

    ///
    /// \brief get_decay_level
    ///
    real_t get_decay_level(uint_t update_idx, uint_t num_updates)const;

protected:

    ///
    /// \brief A2C Constructor
    ///
    A2C(world_t& environment, policies::TorchPolicy& policy, const A2CInput input);

    ///
    /// \brief world
    ///
    world_t* world(){return world_ptr_;}

    ///
    /// \brief policy
    ///
    policies::TorchPolicy& policy(){return policy_;}

private:

    ///
    /// \brief The world used by the agent
    ///
    world_t* world_ptr_;

    ///
    /// \brief policy
    ///
    policies::TorchPolicy& policy_;

    ///
    /// \brief input_
    ///
    A2CInput input_;

    ///
    /// \brief optimizer_
    ///
    std::unique_ptr<torch::optim::RMSprop> optimizer_;

};

template<typename WorldTp>
A2C<WorldTp>::A2C(world_t& comm, policies::TorchPolicy& policy, const A2CInput input)
    :
     policy_(policy),
     input_(input),
     optimizer_(std::make_unique<torch::optim::RMSprop>(
                    policy->parameters(),
                    torch::optim::RMSpropOptions(input.learning_rate)
                        .eps(input.epsilon)
                        .alpha(input.gamma)))
{}

template<typename WorldTp>
void
A2C<WorldTp>::decay_learning_rate(value_t decay_level){


    for (auto &group : optimizer_->param_groups())
        {
            if(group.has_options())
            {
                auto &options = static_cast<torch::optim::RMSpropOptions &>(group.options());
                options.lr(input_.original_learning_rate * decay_level);
            }
        }
}

template<typename WorldTp>
real_t
A2C<WorldTp>::get_decay_level(uint_t update_idx, uint_t num_updates)const{

    if(input_.use_decay_lr){
        return 1. - static_cast<real_t>(update_idx) / num_updates;
    }

    return 1.0;
}

template<typename WorldTp>
std::vector<utils::UpdateDatum<typename A2C<WorldTp>::value_t>>
A2C<WorldTp>::update(utils::TorchRolloutStorage &rollouts, value_t decay_level){

    // Decay learning rate
    decay_learning_rate(decay_level);
    //optimizer_->defaults().lr() = (input_.original_learning_rate * decay_level);

    // Prep work
    auto full_obs_shape = rollouts.get_observations().sizes();
    std::vector<int64_t> obs_shape(full_obs_shape.begin() + 2,
                                   full_obs_shape.end());

    obs_shape.insert(obs_shape.begin(), -1);
    auto action_shape = rollouts.get_actions().size(-1);
    auto rewards_shape = rollouts.get_rewards().sizes();
    int num_steps = rewards_shape[0];
    int num_processes = rewards_shape[1];

    // Update observation normalizer
    if (policy_->using_observation_normalizer()){
        policy_->update_observation_normalizer(rollouts.get_observations());
    }

    // Run evaluation on rollouts
    auto evaluate_result = policy_->evaluate_actions(
                        rollouts.get_observations().slice(0, 0, -1).view(obs_shape),
                        rollouts.get_hidden_states()[0].view({-1, static_cast<long>(policy_->get_hidden_size())}),
                        rollouts.get_masks().slice(0, 0, -1).view({-1, 1}),
                        rollouts.get_actions().view({-1, action_shape}));

    auto values = evaluate_result[0].view({num_steps, num_processes, 1});
    auto action_log_probs = evaluate_result[1].view({num_steps, num_processes, 1});

    // Calculate advantages
    // Advantages aren't normalized (they are in PPO)
    auto advantages = rollouts.get_returns().slice(0, 0, -1) - values;

    // Value loss
    auto value_loss = advantages.pow(2).mean();

    // Action loss
    auto action_loss = -(advantages.detach() * action_log_probs).mean();

    // Total loss
    auto loss = (value_loss * input_.value_loss_coef +
                 action_loss -
                 evaluate_result[2] * input_.entropy_coef);

    // Step optimizer
    optimizer_->zero_grad();
    loss.backward();
    optimizer_->step();

    return {utils::UpdateDatum<A2C::value_t>("Value loss", value_loss.item().toFloat()),
            utils::UpdateDatum<A2C::value_t>("Action loss", action_loss.item().toFloat()),
            utils::UpdateDatum<A2C::value_t>("Entropy", evaluate_result[2].item().toFloat())};

}

template<typename WorldTp>
void
A2C<WorldTp>::train(){

}

}

}

#endif
#endif // A2C_H
