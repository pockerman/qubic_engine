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
TorchPolicyImpl::act(torch::Tensor inputs,
                               torch::Tensor rnn_hxs,
                     torch::Tensor masks) const{}


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
