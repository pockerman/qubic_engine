#include "cubic_engine/rl/policies/torch_policy.h"

#ifdef USE_PYTORCH

#include "cubic_engine/ml/neural_networks/torch_output_layers.h"
#include <stdexcept>

namespace cengine {
namespace rl {
namespace policies {


TorchPolicyImpl::TorchPolicyImpl(actions::ActionSpace action_space,
                                 std::shared_ptr<nets::TorchNNBase> base,
                                 bool normalize_observations)
    :
      action_space_(action_space),
      base_(register_module("base", base)),
      observation_normalizer_(nullptr),
      output_layer_()
{
    int num_outputs = action_space.shape[0];
    if (action_space.type == "Discrete"){
        output_layer_ = std::make_shared<cengine::ml::nets::CategoricalOutput>(
        base_->get_output_size(), num_outputs);
    }
    else if (action_space.type == "Box"){
        output_layer_ = std::make_shared<cengine::ml::nets::NormalOutput>(
        base_->get_output_size(), num_outputs);
    }
    else if (action_space.type == "MultiBinary"){
        output_layer_ = std::make_shared<cengine::ml::nets::BernoulliOutput>(
        base_->get_output_size(), num_outputs);
    }
    else{
        throw std::runtime_error("Action space " + action_space.type + " not supported");
    }

    register_module("output", output_layer_);

    if (normalize_observations)
    {
        throw std::runtime_error("Not implemented");
        // Normalized observations only supported for MlpBase
        //auto mlp_base = static_cast<MlpBase *>(base.get());
        //observation_normalizer_ = register_module("observation_normalizer", ObservationNormalizer(mlp_base->get_num_inputs()));
    }
}


std::vector<torch::Tensor>
TorchPolicyImpl::act(torch::Tensor inputs, torch::Tensor rnn_hxs, torch::Tensor masks) const{

    if (observation_normalizer_){
            inputs = observation_normalizer_->process_observation(inputs);
    }

    auto base_output = base_->forward(inputs, rnn_hxs, masks);
    auto dist = output_layer_->forward(base_output[1]);

    auto action = dist->sample({});
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
