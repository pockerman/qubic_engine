#ifndef TORCH_RECURRENT_GENERATOR_H
#define TORCH_RECURRENT_GENERATOR_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH


#include "cubic_engine/rl/utils/torch_generator_base.h"
#include "cubic_engine/rl/utils/torch_mini_batch.h"

namespace cengine {
namespace rl {
namespace utils {

class TorchRecurrentGenerator: public TorchGeneratorBase
{
public:

    ///
    /// \brief TorchFeedForwardGenerator
    ///
    TorchRecurrentGenerator(int mini_batch_size, int num_envs_per_batch_,
                             torch::Tensor observations,
                             torch::Tensor hidden_states,
                             torch::Tensor actions,
                             torch::Tensor value_predictions,
                             torch::Tensor returns,
                             torch::Tensor masks,
                             torch::Tensor action_log_probs,
                             torch::Tensor advantages);

    ///
    /// \brief done
    /// \return
    ///
    virtual bool done() const override final;

    ///
    /// \brief next
    /// \return
    ///
    virtual TorchMiniBatch next() override final;

private:

    int index_;
    int num_envs_per_batch_;
    torch::Tensor observations_;
    torch::Tensor hidden_states_;
    torch::Tensor actions_;
    torch::Tensor value_predictions_;
    torch::Tensor returns_;
    torch::Tensor masks_;
    torch::Tensor action_log_probs_;
    torch::Tensor advantages_;
    torch::Tensor indices_;


};

}
}
}

#endif
#endif // TORCH_RECURRENT_GENERATOR_H
