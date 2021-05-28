#ifndef TORCH_DISTRIBUTION_H
#define TORCH_DISTRIBUTION_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "kernel/base/types.h"
#include "torch/torch.h"

#include <vector>

namespace kernel {
namespace numerics {
namespace stats {

///
/// \brief The TorchDistribution class
///
class TorchDistribution
{

public:

    ///
    ///
    ///
    virtual ~TorchDistribution() = default;

    ///
    /// \brief entropy
    ///
    virtual torch::Tensor entropy() = 0;

    ///
    /// \brief log_prob
    /// \param value
    /// \return
    ///
    virtual torch::Tensor log_prob(torch::Tensor value) = 0;

    ///
    ///
    ///
    virtual torch::Tensor sample(c10::ArrayRef<int64_t> sample_shape) = 0;

protected:

    ///
    /// \brief batch_shape_
    ///
    std::vector<int64_t> batch_shape_;

    ///
    /// \brief event_shape_
    ///
    std::vector<int64_t> event_shape_;

    ///
    /// \brief extended_shape
    /// \param sample_shape
    /// \return
    ///
    std::vector<int64_t> extended_shape(c10::ArrayRef<int64_t> sample_shape);

};

}
}
}
#endif
#endif // TORCH_DISTRIBUTION_H
