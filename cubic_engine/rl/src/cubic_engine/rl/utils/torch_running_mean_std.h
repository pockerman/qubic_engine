#ifndef TORCH_RUNNIN_MEAN_STD_H
#define TORCH_RUNNIN_MEAN_STD_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"

#include "torch/torch.h"
#include <vector>

namespace cengine{
namespace rl {
namespace utils {

// https://github.com/openai/baselines/blob/master/baselines/common/running_mean_std.py
class TorchRunningMeanStdImpl : public torch::nn::Module
{

public:

    ///
    /// \brief TorchRunningMeanStdImpl
    /// \param size
    ///
    explicit TorchRunningMeanStdImpl(int size);

    ///
    /// \brief TorchRunningMeanStdImpl
    ///
    TorchRunningMeanStdImpl(std::vector<float> means,
                            std::vector<float> variances);

    ///
    /// \brief update
    /// \param observation
    ///
    void update(const torch::Tensor& observation);

    ///
    /// \brief get_count
    /// \return
    ///
    int get_count() const { return static_cast<int>(count.item().toFloat()); }

    ///
    /// \brief get_mean
    /// \return
    ///
    torch::Tensor get_mean() const { return mean.clone(); }

    ///
    /// \brief get_variance
    /// \return
    ///
    torch::Tensor get_variance() const { return variance.clone(); }

    ///
    /// \brief set_count
    /// \param count
    ///
    void set_count(int count) { this->count[0] = count + 1e-8; }

private:

    ///
    /// \brief count
    ///
    torch::Tensor count;

    ///
    /// \brief mean
    ///
    torch::Tensor mean;

    ///
    /// \brief variance
    ///
    torch::Tensor variance;

    ///
    /// \brief update_from_moments_
    ///
    void update_from_moments_(const torch::Tensor& batch_mean,
                              const  torch::Tensor& batch_var,
                               uint_t batch_count);

};

TORCH_MODULE(TorchRunningMeanStd);

}

}
}

#endif
#endif // TORCH_RUNNIN_MEAN_STD_H
