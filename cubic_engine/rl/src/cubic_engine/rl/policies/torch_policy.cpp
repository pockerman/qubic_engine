#include "cubic_engine/rl/policies/torch_policy.h"

#ifdef USE_PYTORCH

namespace cengine {
namespace rl {
namespace policies {


TorchPolicyImpl::TorchPolicyImpl(actions::ActionSpace action_space,
                std::shared_ptr<nets::TorchNNBase> base, bool normalize_observations)
    :
      action_space_(action_space),
      base_(register_module("base", base)),
      observation_normalizer_(nullptr)
{}


std::vector<torch::Tensor>
TorchPolicyImpl::act(torch::Tensor inputs, torch::Tensor rnn_hxs, torch::Tensor masks) const{

    if (observation_normalizer_){
            inputs = observation_normalizer_->process_observation(inputs);
    }

    auto base_output = base_->forward(inputs, rnn_hxs, masks);
    auto dist = output_layer_->forward(base_output[1]);

    auto action = dist->sample();
    auto action_log_probs = dist->log_prob(action);

    if (action_space_.type == "Discrete")
    {
        action = action.unsqueeze(-1);
        action_log_probs = action_log_probs.unsqueeze(-1);
    }
    else
    {
        action_log_probs = dist->log_prob(action).sum(-1, true);
    }

    return {base_output[0], // value
                action,
                action_log_probs,
                base_output[2]}; // rnn_hxs

}


std::vector<torch::Tensor>
TorchPolicyImpl::evaluate_actions(torch::Tensor inputs,
                                  torch::Tensor rnn_hxs,
                                  torch::Tensor masks,
                                  torch::Tensor actions) const{}

torch::Tensor
TorchPolicyImpl::get_probs(torch::Tensor inputs,
                        torch::Tensor rnn_hxs,
                           torch::Tensor masks) const{}


torch::Tensor
TorchPolicyImpl::get_values(torch::Tensor inputs,
                         torch::Tensor rnn_hxs,
                            torch::Tensor masks) const{}


void
TorchPolicyImpl::update_observation_normalizer(torch::Tensor observations){}


}
}
}

#endif
