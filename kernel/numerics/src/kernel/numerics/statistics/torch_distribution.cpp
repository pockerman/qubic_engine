#include "kernel/numerics/statistics/torch_distribution.h"

#ifdef USE_PYTORCH

namespace kernel {
namespace numerics {
namespace stats {

std::vector<int64_t>
TorchDistribution::extended_shape(c10::ArrayRef<int64_t> sample_shape)
{
    std::vector<int64_t> output_shape;
    output_shape.insert(output_shape.end(),
                        sample_shape.begin(),
                        sample_shape.end());
    output_shape.insert(output_shape.end(),
                        batch_shape_.begin(),
                        batch_shape_.end());
    output_shape.insert(output_shape.end(),
                        event_shape_.begin(),
                        event_shape_.end());
    return output_shape;
}

}
}
}

#endif
