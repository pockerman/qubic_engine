#ifndef TORCH_CNN_BASE_H
#define TORCH_CNN_BASE_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/rl/networks/torch_nn.h"
#include <vector>

namespace cengine {
namespace rl {
namespace nets{


///
/// \brief The TorchCNNBase class Basic Torch CNN network
///
class TorchCNNBase: public TorchNNBase
{
public:

    ///
    /// \brief TorchCNNBase
    /// \param num_inputs
    /// \param hidden_size
    ///
    TorchCNNBase(uint_t num_inputs,
                 uint_t output_size = 512);

    ///
    /// \brief forward
    /// \param inputs
    /// \param hxs
    /// \param masks
    /// \return
    ///
    virtual std::vector<torch::Tensor> forward(torch::Tensor inputs,
                                               torch::Tensor hxs,
                                               torch::Tensor masks) override;

private:

    ///
    /// \brief main_
    ///
    torch::nn::Sequential main_;

    ///
    /// \brief critic_linear_
    ///
    torch::nn::Sequential critic_linear_;


};

}

}
}
#endif
#endif // TORCH_CNN_BASE_H
