#ifndef TORCH_NORMAL_H
#define TORCH_NORMAL_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "kernel/base/types.h"
#include "kernel/numerics/statistics/torch_distribution.h"
#include "torch/torch.h"


namespace kernel {
namespace numerics {
namespace stats {

///
/// \brief The TorchDistribution class
///
class TorchNormalDistribution final : public TorchDistribution
{

  public:

    ///
    /// \brief TorchNormalDistribution
    ///
    TorchNormalDistribution(const torch::Tensor loc,
                            const torch::Tensor scale);

    ///
    ///
    ///
    ~TorchNormalDistribution() = default;

    ///
    /// \brief entropy
    ///
    virtual torch::Tensor entropy() override final;

    ///
    /// \brief log_prob
    /// \param value
    /// \return
    ///
    virtual torch::Tensor log_prob(torch::Tensor value) override final;

    ///
    ///
    ///
    virtual torch::Tensor sample(c10::ArrayRef<int64_t> sample_shape) override final;

protected:

    ///
    /// \brief loc
    ///
    torch::Tensor loc_;

    ///
    /// \brief scale
    ///
    torch::Tensor scale_;

};

}
}
}
#endif

#endif // TORCH_NORMAL_H
