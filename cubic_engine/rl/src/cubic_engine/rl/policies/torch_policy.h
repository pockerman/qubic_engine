#ifndef TORCH_POLICY_H
#define TORCH_POLICY_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/actions/action_space.h"
#include "cubic_engine/rl/utils/torch_observation_normalizer.h"
#include "cubic_engine/rl/networks/torch_nn.h"
#include "cubic_engine/ml/neural_networks/torch_output_layers.h"
#include "torch/torch.h"

#include <memory>
#include <vector>

namespace cengine {
namespace rl {
namespace policies {



///
/// \brief The TorchPolicyImpl class
///
class TorchPolicyImpl: public torch::nn::Module
{

public:

    ///
    /// \brief TorchPolicyImpl
    /// \param action_space
    /// \param base
    /// \param normalize_observations
    ///
    TorchPolicyImpl(actions::ActionSpace action_space,
                    std::shared_ptr<nets::TorchNNBase> base,
                    bool normalize_observations = false);

    ///
    /// \brief act
    /// \param inputs
    /// \param rnn_hxs
    /// \param masks
    /// \return
    ///
    std::vector<torch::Tensor> act(torch::Tensor inputs,
                                   torch::Tensor rnn_hxs,
                                   torch::Tensor masks) const;

    ///
    /// \brief evaluate_actions
    /// \param inputs
    /// \param rnn_hxs
    /// \param masks
    /// \param actions
    /// \return
    ///
    std::vector<torch::Tensor> evaluate_actions(torch::Tensor inputs,
                                                torch::Tensor rnn_hxs,
                                                torch::Tensor masks,
                                                torch::Tensor actions) const;

    ///
    /// \brief get_probs
    /// \param inputs
    /// \param rnn_hxs
    /// \param masks
    /// \return
    ///
    torch::Tensor get_probs(torch::Tensor inputs,
                            torch::Tensor rnn_hxs,
                            torch::Tensor masks) const;

    ///
    /// \brief get_values
    /// \param inputs
    /// \param rnn_hxs
    /// \param masks
    /// \return
    ///
    torch::Tensor get_values(torch::Tensor inputs,
                             torch::Tensor rnn_hxs,
                             torch::Tensor masks) const;

    ///
    /// \brief update_observation_normalizer
    /// \param observations
    ///
    void update_observation_normalizer(torch::Tensor observations);

    ///
    /// \brief is_recurrent
    /// \return
    ///
    //bool is_recurrent() const { return base->is_recurrent(); }

    ///
    /// \brief get_hidden_size
    /// \return
    ///
    uint_t get_hidden_size() const{return base_->get_hidden_size();}

    ///
    /// \brief using_observation_normalizer
    /// \return
    ///
    bool using_observation_normalizer() const{return !observation_normalizer_.is_empty();}

private:

    ///
    /// \brief action_space_
    ///
    actions::ActionSpace action_space_;

    ///
    /// \brief base_
    ///
    std::shared_ptr<nets::TorchNNBase> base_;

    ///
    /// \brief observation_normalizer_
    ///
    utils::TorchObservationNormalizer observation_normalizer_;

    ///
    /// \brief output_layer_
    ///
    std::shared_ptr<cengine::ml::nets::LinearOutputLayer> output_layer_;

    std::vector<torch::Tensor> forward_gru(torch::Tensor x,
                                           torch::Tensor hxs,
                                           torch::Tensor masks);

};

TORCH_MODULE(TorchPolicy);
}

}

}
#endif
#endif // TORCH_POLICY_H
