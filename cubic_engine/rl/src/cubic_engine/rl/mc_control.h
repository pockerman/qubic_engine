#ifndef MC_CONTROL_H
#define MC_CONTROL_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/rl_algorithm_base.h"
#include "cubic_engine/rl/greedy_policy.h"
#include "kernel/utilities/iterative_algorithm_controller.h"

#include <vector>
#include <tuple>

namespace cengine{
namespace rl {

///
/// \brief The MCControl class. Monte Carlo control
/// with importance sampling
///
template<typename WorldTp, typename PolicySamplerTp, typename TargetPolicyTp=GreedyPolicy>
class MCControl: public RLAlgorithmBase<WorldTp>{

public:

    ///
    /// \brief The type of the world
    ///
    typedef typename RLAlgorithmBase<WorldTp>::world_t world_t;

    ///
    /// \brief The type of the action
    ///
    typedef typename world_t::action_t action_t;

    ///
    /// \brief The type of the reward
    ///
    typedef typename world_t::reward_value_t reward_value_t;

    ///
    /// \brief The type of the state
    ///
    typedef typename world_t::state_t state_t;

    ///
    /// \brief policy_sampler_t. The type of the object
    /// that handles policy sampling
    ///
    typedef PolicySamplerTp policy_sampler_t;

    ///
    /// \brief target_policy_t The type of the target policy
    ///
    typedef TargetPolicyTp target_policy_t;

    ///
    /// \brief MCControl. Constructor
    ///
    MCControl(const RLIterativeAlgoInput& input);

    ///
    /// \brief step. Performs the iterations for
    /// one training episode
    ///
    virtual void step()override final;

    ///
    /// \brief Initialize the underlying data structures
    ///
    virtual void initialize(world_t& world, reward_value_t val);

private:

    ///
    /// \brief episode_ A tuple representing an episode
    ///
    std::vector<std::tuple<state_t*,  action_t, real_t>> episodes_;

    ///
    /// \brief q_function_
    ///
    DynMat<real_t> q_function_;

    ///
    /// \brief q_function_
    ///
    DynMat<real_t> c_;

    ///
    /// \brief behavior_policy_. Behavior policy
    ///
    policy_sampler_t behavior_policy_;

    ///
    /// \brief target_policy_ The target policy
    ///
    target_policy_t target_policy_;

};

template<typename WorldTp, typename PolicySamplerTp, typename TargetPolicyTp>
MCControl<WorldTp, PolicySamplerTp, TargetPolicyTp>::MCControl(const RLIterativeAlgoInput& input)
    :
     RLAlgorithmBase<WorldTp>(input),
     episodes_(),
     q_function_(),
     c_(),
     behavior_policy_(q_function_),
     target_policy_(q_function_)

{}


template<typename WorldTp, typename PolicySamplerTp, typename TargetPolicyTp>
void
MCControl<WorldTp, PolicySamplerTp, TargetPolicyTp>::initialize(world_t& world, reward_value_t val){

    q_function_.resize(world.n_states(), world.n_actions(), val);
    c_.resize(world.n_states(), world.n_actions(), val);

    // finally set the world pointer
    this->world_ptr_ = &world;
    this->state_ = nullptr;
    this->is_initialized_ = true;
}

template<typename WorldTp, typename PolicySamplerTp, typename TargetPolicyTp>
void
MCControl<WorldTp, PolicySamplerTp, TargetPolicyTp>::step(){

    for(uint_t itr=0; itr< this->get_total_itrs_per_episode(); ++itr){

        // get the action from the behavior
        auto action = behavior_policy_.get_action_index(*this->state_);

        // step in the world
        auto [new_state, reward, finished, info] = this->world_ptr_->step(static_cast<action_t>(action));
        episodes_.push_back(std::make_tuple(new_state, static_cast<action_t>(action), reward));

        if(finished){
            break;
        }

        this->state_ = new_state;
    }

    // Sum of discounted returns
    auto G = 0.0;

    // The importance sampling ratio (the weights of the returns)
    auto W = 1.0;

    for(uint_t e=0; e<episodes_.size(); ++e){

        const auto& state = episodes_[e];

        auto action = std::get<1>(state);
        auto state_idx = std::get<0>(state)->get_id();

        // Update total reward
        G = this->get_discount_factor() * G + std::get<2>(state);

        // Update weighted importance sampling formula denominator
        c_(state_idx, static_cast<uint_t>(action)) += W;

        // Update the action-value function using the incremental update formula (5.7)
        // This also improves our target policy which holds a reference to Q
        q_function_(state_idx,
                    static_cast<uint_t>(action)) += (W / c_(state_idx,
                                                            static_cast<uint_t>(action))) * (G - q_function_(state_idx,
                                                                                                             static_cast<uint_t>(action)));

        // If the action taken by the behavior policy is not the action
        // taken by the target policy the probability will be 0 and we can break
        if( static_cast<uint_t>(action) != target_policy_.get_action_index(*std::get<0>(state))){
            break;
        }

        W = W * 1. / behavior_policy_.get_probabilities(*std::get<0>(state))[static_cast<uint_t>(action)];

    }
}


}
}

#endif // MC_CONTROL_H
