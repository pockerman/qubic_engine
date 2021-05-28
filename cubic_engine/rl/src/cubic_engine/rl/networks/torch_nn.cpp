#include "cubic_engine/rl/networks/torch_nn.h"

#ifdef USE_PYTORCH

namespace cengine {
namespace rl {
namespace nets{


TorchNNBase::TorchNNBase(uint_t output_size)
    :
      torch::nn::Module(),
      gru_(nullptr),
      output_size_(output_size)
{}

}
}
}

#endif
