#include "cubic_engine/rl/utils/torch_mini_batch.h"

#ifdef USE_PYTORCH
namespace cengine{
namespace rl{
namespace utils{


MiniBatch<kernel::torch_t>::MiniBatch(torch::Tensor observations,
                                      torch::Tensor hidden_states,
                                      torch::Tensor actions,
                                      torch::Tensor value_predictions,
                                      torch::Tensor returns,
                                      torch::Tensor masks,
                                      torch::Tensor action_log_probs,
                                      torch::Tensor advantages)
    :
      observations(observations),
      hidden_states(hidden_states),
      actions(actions),
      value_predictions(value_predictions),
      returns(returns),
      masks(masks),
      action_log_probs(action_log_probs),
      advantages(advantages)
{}

}
}
}

#endif
