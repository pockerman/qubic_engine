#include "cubic_engine/rl/networks/torch_nn.h"

#ifdef USE_PYTORCH

namespace cengine {
namespace rl {
namespace nets{


TorchNNBase::TorchNNBase()
    :
      torch::nn::Module(),
      gru_(nullptr),
      hidden_size_()
{}


uint_t
TorchNNBase::get_hidden_size() const{}

}
}
}

#endif
