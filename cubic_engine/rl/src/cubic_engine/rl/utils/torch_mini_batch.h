#ifndef TORCH_MINI_BATCH_H
#define TORCH_MINI_BATCH_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "kernel/base/types.h"
#include <torch/torch.h>

namespace cengine {
namespace rl {
namespace utils {

/// forward declarations
template<typename BatchTp> struct MiniBatch;

template<>
struct MiniBatch<kernel::torch_t>
{

  ///
  /// \brief Constructor
  ///
  MiniBatch() = default;

  ///
  /// \brief MiniBatch
  /// \param observations
  /// \param hidden_states
  /// \param actions
  /// \param value_predictions
  /// \param returns
  /// \param masks
  /// \param action_log_probs
  /// \param advantages
  ///
  MiniBatch(torch::Tensor observations,
            torch::Tensor hidden_states,
            torch::Tensor actions,
            torch::Tensor value_predictions,
            torch::Tensor returns,
            torch::Tensor masks,
            torch::Tensor action_log_probs,
            torch::Tensor advantages);


   torch::Tensor observations;
   torch::Tensor hidden_states;
   torch::Tensor actions;
   torch::Tensor value_predictions;
   torch::Tensor returns;
   torch::Tensor masks;
   torch::Tensor action_log_probs;
   torch::Tensor advantages;

};

//
using TorchMinBatch = MiniBatch<kernel::torch_t>;

}

}

}

#endif
#endif // TORCH_MINI_BATCH_H
