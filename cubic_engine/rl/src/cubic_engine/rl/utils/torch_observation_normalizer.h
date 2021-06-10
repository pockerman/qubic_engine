#ifndef TORCH_OBSERVATION_NORMALIZER_H
#define TORCH_OBSERVATION_NORMALIZER_H

#include  "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/utils/torch_running_mean_std.h"

#include "torch/torch.h"
#include <vector>

namespace cengine {
namespace rl {
namespace utils {



///
/// \brief The ObservationNormalizerImpl class
///
class TorchObservationNormalizerImpl : public torch::nn::Module
{

public:

    typedef real_t value_t;

    ///
    /// \brief ObservationNormalizerImpl
    /// \param size
    /// \param clip
    ///
    explicit TorchObservationNormalizerImpl(int size, value_t clip = 10.);

    ///
    /// \brief ObservationNormalizerImpl
    /// \param means
    /// \param variances
    /// \param clip
    ///
    TorchObservationNormalizerImpl(const std::vector<value_t> &means,
                              const std::vector<value_t> &variances,
                                   value_t clip = 10.){}

    ///
    /// \brief ObservationNormalizerImpl
    /// \param others
    ///
    explicit TorchObservationNormalizerImpl(const std::vector<TorchObservationNormalizerImpl> &others){}

    ///
    /// \brief process_observation
    /// \param observation
    /// \return
    ///
    torch::Tensor process_observation(torch::Tensor observation) const{}

    ///
    /// \brief get_mean
    /// \return
    ///
    std::vector<value_t> get_mean() const{}

    ///
    /// \brief get_variance
    /// \return
    ///
    std::vector<value_t> get_variance() const{}

    ///
    /// \brief update
    /// \param observations
    ///
    void update(torch::Tensor observations){}

    ///
    /// \brief get_clip_value
    /// \return
    ///
    value_t get_clip_value() const { return clip.item().toFloat(); }

    ///
    /// \brief get_step_count
    /// \return
    ///
    uint_t get_step_count() const { return rms->get_count(); }

private:

    ///
    /// \brief clip
    ///
    torch::Tensor clip;

    ///
    /// \brief rms
    ///
    TorchRunningMeanStd rms{nullptr};

};


TORCH_MODULE(TorchObservationNormalizer);

}

}

}

#endif
#endif // TORCH_OBSERVATION_NORMALIZER_H
