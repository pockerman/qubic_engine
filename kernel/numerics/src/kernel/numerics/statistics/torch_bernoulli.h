#ifndef TORCH_BERNOULLI_H
#define TORCH_BERNOULLI_H
#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "kernel/base/types.h"
#include "kernel/numerics/statistics/torch_distribution.h"
#include "torch/torch.h"

namespace kernel {
namespace numerics {
namespace stats {


class TorchBernoulli final : public TorchDistribution
{


public:

    ///
    /// \brief TorchBernoulli
    /// \param probs
    /// \param logits
    ///
    TorchBernoulli(const torch::Tensor *probs, const torch::Tensor *logits);

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
    virtual torch::Tensor log_prob(torch::Tensor value)override;

    ///
    /// \brief sample
    /// \param sample_shape
    /// \return
    ///
    virtual torch::Tensor sample(c10::ArrayRef<int64_t> sample_shape) override;

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
};


}
}
}

#endif
#endif
