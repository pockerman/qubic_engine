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
    TorchNNBase();

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


    uint_t get_hidden_size() const;
    uint_t get_output_size() const { return hidden_size_; }


private:

    torch::nn::GRU gru_;
    uint_t hidden_size_;

};

}
}
}

#endif
#endif // TORCH_NN_H
