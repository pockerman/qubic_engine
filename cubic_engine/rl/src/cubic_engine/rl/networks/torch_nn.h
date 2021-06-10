#ifndef TORCH_NN_H
#define TORCH_NN_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "torch/torch.h"

#include <vector>

namespace cengine {
namespace rl {
namespace nets{

class TorchNNBase: public torch::nn::Module
{
public:

    ///
    /// \brief TorchNNBase
    /// \param recurrent
    /// \param recurrent_input_size
    /// \param hidden_size
    ///
    TorchNNBase(uint_t output_size);

    ///
    /// \brief forward
    /// \param inputs
    /// \param hxs
    /// \param masks
    /// \return
    ///
    virtual std::vector<torch::Tensor> forward(torch::Tensor inputs,
                                               torch::Tensor hxs,
                                               torch::Tensor masks)=0;

    ///
    /// \brief get_hidden_size
    /// \return
    ///
    virtual int get_hidden_size() const{ return 1;}

    ///
    /// \brief get_output_size
    /// \return
    ///
    int get_output_size() const { return static_cast<int>(output_size_); }

private:

    torch::nn::GRU gru_;

    ///
    /// \brief output_size_
    ///
    uint_t output_size_;

};

}
}
}

#endif
#endif // TORCH_NN_H
