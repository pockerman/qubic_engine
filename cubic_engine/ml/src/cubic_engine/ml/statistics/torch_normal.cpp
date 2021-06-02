#include "cubic_engine/ml/statistics/torch_normal.h"

#ifdef USE_PYTORCH

#include <cmath>

namespace cengine {
namespace ml {
namespace stats {

TorchNormalDistribution::TorchNormalDistribution(const torch::Tensor loc,
                                                 const torch::Tensor scale)
    :
      TorchDistribution(),
      loc_(),
      scale_()
{
    auto broadcasted_tensors = torch::broadcast_tensors({loc, scale});
    loc_ = broadcasted_tensors[0];
    scale_ = broadcasted_tensors[1];
    batch_shape_ = loc_.sizes().vec();
    event_shape_ = {};
}

TorchNormalDistribution::~TorchNormalDistribution()
{}

torch::Tensor
TorchNormalDistribution::entropy(){
        return (0.5 + 0.5 * std::log(2 * M_PI) + torch::log(scale_)).sum(-1);
}

torch::Tensor
TorchNormalDistribution::log_prob(torch::Tensor value){

    auto variance = scale_.pow(2);
    auto log_scale = scale_.log();
    return (-(value - loc_).pow(2) /
                    (2 * variance) -
                log_scale -
                std::log(std::sqrt(2 * M_PI)));

}

torch::Tensor
TorchNormalDistribution::sample(c10::ArrayRef<int64_t> sample_shape){

    auto shape = extended_shape(sample_shape);
    auto no_grad_guard = torch::NoGradGuard();
    return at::normal(loc_.expand(shape), scale_.expand(shape));

}


}
}
}

#endif
