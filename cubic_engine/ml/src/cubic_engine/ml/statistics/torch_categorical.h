#ifndef TORCH_CATEGORICAL_H
#define TORCH_CATEGORICAL_H

#include "kernel/base/config.h"

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "kernel/base/types.h"
#include "cubic_engine/ml/statistics/torch_distribution.h"
#include "torch/torch.h"


namespace cengine {
namespace ml {
namespace stats {

class TorchCategorical final : public TorchDistribution
{


public:

    ///
    /// \brief TorchCategorical
    /// \param probs
    /// \param logits
    ///
    TorchCategorical(const torch::Tensor *probs, const torch::Tensor *logits);
    
    
    virtual ~TorchCategorical();

    ///
    /// \brief entropy
    /// \return
    ///
    virtual torch::Tensor entropy() override;

    ///
    /// \brief log_prob
    /// \param value
    /// \return
    ///
    virtual torch::Tensor log_prob(torch::Tensor value) override;

    ///
    ///
    ///
    virtual torch::Tensor sample(c10::ArrayRef<int64_t> sample_shape = {});

    ///
    /// \brief get_logits
    /// \return
    ///
    torch::Tensor get_logits()const { return logits_; }

    ///
    /// \brief get_probs
    /// \return
    ///
    torch::Tensor get_probs()const { return probs_; }

private:
  torch::Tensor probs_;
  torch::Tensor logits_;
  torch::Tensor param_;
  int num_events_;

};
}
}
}
#endif
#endif
