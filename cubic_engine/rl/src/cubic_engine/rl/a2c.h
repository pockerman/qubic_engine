#ifndef A2C_H
#define A2C_H

#include "kernel/base/config.h"

#ifdef USE_PYTORCH

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/utils/rollout_storage.h"


#include "torch/torch.h"
#include <memory>
#include <vector>

namespace cengine {
namespace rl {

/// Forward declarations
class Policy;


namespace utils {

//class RolloutStorage;
struct UpdateDatum;

}

///
/// \brief The A2CInput struct
///
struct A2CInput
{
    typedef real_t value_t ;
    value_t actor_loss_coef;
    value_t value_loss_coef;
    value_t entropy_coef;
    value_t learning_rate;
    value_t original_learning_rate;
    value_t epsilon{1e-8};
    value_t alpha{0.99};
    value_t max_grad_norm{0.5};

};

///
/// \brief The A2C class. PyTorch based implementation of A2C algorithm
///
class A2C
{

  public:

    typedef real_t value_t ;

    ///
    /// \brief A2C Constructor
    /// \param policy
    /// \param actor_loss_coef
    /// \param value_loss_coef
    /// \param entropy_coef
    /// \param learning_rate
    /// \param epsilon
    /// \param alpha
    /// \param max_grad_norm
    ///
    A2C(Policy &policy,
        const A2CInput input);

    ///
    /// \brief update
    /// \param rollouts
    /// \param decay_level
    /// \return
    ///
    std::vector<utils::UpdateDatum> update(utils::TorchRolloutStorage &rollouts, value_t decay_level = 1);


private:

    ///
    /// \brief policy
    ///
    Policy& policy_;

    ///
    /// \brief input_
    ///
    A2CInput input_;

    ///
    /// \brief optimizer_
    ///
    std::unique_ptr<torch::optim::RMSprop> optimizer_;

};

}

}

#endif
#endif // A2C_H
