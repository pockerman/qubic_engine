#include "cubic_engine/rl/utils/torch_running_mean_std.h"

#ifdef USE_PYTORCH

namespace cengine{
namespace rl{
namespace utils{

TorchRunningMeanStdImpl::TorchRunningMeanStdImpl(int size)
    :
      torch::nn::Module(),
      count(register_buffer("count", torch::full({1}, 1e-4, torch::kFloat))),
      mean(register_buffer("mean", torch::zeros({size}))),
      variance(register_buffer("variance", torch::ones({size})))
{}

TorchRunningMeanStdImpl::TorchRunningMeanStdImpl(std::vector<float> means,
                                                 std::vector<float> variances)
    :
      torch::nn::Module(),
      count(register_buffer("count", torch::full({1}, 1e-4, torch::kFloat))),
      mean(register_buffer("mean",
                           torch::from_blob(means.data(),
                                            {static_cast<long>(means.size())}).clone())),
      variance(register_buffer("variance",
                               torch::from_blob(variances.data(),
                                                {static_cast<long>(variances.size())}).clone()))
{}

void
TorchRunningMeanStdImpl::update(const torch::Tensor& observation){

    auto obs = observation;
    obs = obs.reshape({-1, mean.size(0)});
    auto batch_mean = obs.mean(0);
    auto batch_var = obs.var(0, false, false);
    auto batch_count = obs.size(0);

    update_from_moments_(batch_mean, batch_var, batch_count);
}

void
TorchRunningMeanStdImpl::update_from_moments_(const torch::Tensor& batch_mean,
                                              const  torch::Tensor& batch_var,
                                              uint_t batch_count){

    auto delta = batch_mean - mean;
    auto total_count = count + static_cast<int>(batch_count);

    mean.copy_(mean + delta * static_cast<int>(batch_count) / total_count);
    auto m_a = variance * count;
    auto m_b = batch_var * static_cast<int>(batch_count);
    auto m2 = m_a + m_b + torch::pow(delta, 2) * count * static_cast<int>(batch_count) / total_count;
    variance.copy_(m2 / total_count);
    count.copy_(total_count);

}

}
}
}

#endif
