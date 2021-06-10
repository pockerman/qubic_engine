#ifndef TORCH_OUTPUT_LAYERS_H
#define TORCH_OUTPUT_LAYERS_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/statistics/torch_distribution.h"
#include "torch/torch.h"

#include <memory>

namespace cengine {
namespace ml {
namespace nets {

class OutputLayer : public torch::nn::Module
{
  public:

    ///
    /// \brief ~OutputLayer
    ///
    virtual ~OutputLayer() = default;

    ///
    /// \brief forward
    /// \param x
    /// \return
    ///
    virtual std::unique_ptr<ml::stats::TorchDistribution> forward(torch::Tensor x) = 0;
};

class LinearOutputLayer: public OutputLayer
{

public:

	///
    /// \brief ~OutputLayer
    ///
    virtual ~LinearOutputLayer() = default;

    ///
    /// \brief forward
    /// \param x
    /// \return
    ///
    virtual std::unique_ptr<ml::stats::TorchDistribution> forward(torch::Tensor x) = 0;



protected:

    ///
    /// \brief LinearOutputLayer
    /// \param num_inputs
    /// \param num_outputs
    ///
    LinearOutputLayer(uint_t num_inputs, uint_t num_outputs)
        :
         LinearOutputLayer(static_cast<unsigned int>(num_inputs), static_cast<unsigned int>(num_outputs))
        {}

    ///
    /// \brief LinearOutputLayer
    /// \param num_inputs
    /// \param num_outputs
    ///
    LinearOutputLayer(unsigned int num_inputs, unsigned int num_outputs);

    ///
    /// \brief linear
    ///
    torch::nn::Linear linear;

};


class BernoulliOutput : public LinearOutputLayer
{
public:

     ///
    ///
    ///
    BernoulliOutput(unsigned int num_inputs, unsigned int num_outputs);

    ///
    /// \brief BernoulliOutput
    /// \param num_inputs
    /// \param num_outputs
    ///
    BernoulliOutput(uint_t num_inputs, uint_t num_outputs)
        :
        BernoulliOutput(static_cast<unsigned int>(num_inputs), static_cast<unsigned int>(num_outputs))
    {}

    
    
    BernoulliOutput(int num_inputs, int num_outputs)
    :
    BernoulliOutput(static_cast<unsigned int>(num_inputs), static_cast<unsigned int>(num_outputs))
    {}

    ///
    /// \brief forward
    /// \param x
    /// \return
    ///
    std::unique_ptr<ml::stats::TorchDistribution> forward(torch::Tensor x);

};

class CategoricalOutput : public LinearOutputLayer
{
public:

    ///
    /// \brief CategoricalOutput
    /// \param num_inputs
    /// \param num_outputs
    ///
    CategoricalOutput(uint_t num_inputs, uint_t num_outputs);

    ///
    /// \brief forward
    /// \param x
    /// \return
    ///
    std::unique_ptr<ml::stats::TorchDistribution> forward(torch::Tensor x);

};

class NormalOutput : public LinearOutputLayer
{
public:

    ///
    /// \brief NormalOutput
    /// \param num_inputs
    /// \param num_outputs
    ///
    NormalOutput(uint_t num_inputs, uint_t num_outputs);

    ///
    /// \brief forward
    /// \param x
    /// \return
    ///
    std::unique_ptr<ml::stats::TorchDistribution> forward(torch::Tensor x);

private:

  torch::Tensor scale_log;
};

}

}

}

#endif
#endif // TORCH_OUTPUT_LAYERS_H
