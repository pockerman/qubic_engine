#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/td_base.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/matrix_utilities.h"

#if defined(__GNUC__) && (__GNUC___ > 7)
#include "magic_enum.hpp"
#else
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#endif
#include <boost/noncopyable.hpp>
#include <string>

#include <random>
#include <iostream>
#include <cmath>

namespace cengine {
namespace rl{

///
/// \brief The QLearningInput struct
/// Helper struct that assembles the input for the QLearning class.
struct QLearningInput
{
    ///
    /// \breif learning_rate The learning rate
    ///
    real_t learning_rate{0.01};

    ///
    /// \brief epsilon. The exploration coefficient
    ///
    real_t epsilon{1.0};

    ///
    /// \brief max_epsilon. The maximum the exploration
    /// coefficient can get
    ///
    real_t max_epsilon{1.0};

    ///
    /// \brief min_epsilon. The minimum the exploration
    /// coefficient can get
    ///
    real_t min_epsilon{0.01};

    ///
    /// \brief epsilon_decay. The cofficient
    /// used in exponential decay
    ///
    real_t epsilon_decay{0.01};

    ///
    /// \brief discount_factor. The gamma coefficient
    ///
    real_t discount_factor{0.6};

    ///
    /// \brief show_iterations. Flag indicating if informative
    /// messages be printed during training
    ///
    bool show_iterations{true};

    ///
    /// \brief Flag indicating if decaying is used
    ///
    bool use_decay{true};

    ///
    /// \brief max_num_iterations. How many iterations should be
    /// performed per training episode.
    ///
    uint_t max_num_iterations;

    ///
    /// \brief total_episodes. Total number of training episodes
    ///
    uint_t total_episodes;
};


///
/// \brief The QLearning class. Table based implementation
/// of the Q-learning algorithm using epsilon-greedy policy.
/// The implementation also allows for exponential decay
/// of the used epsilon
///
template<typename WorldTp>
class QLearning: public TDBase<WorldTp>
{

public:

    ///
    /// \brief The type of the world
    ///
    typedef WorldTp world_t;

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
    /// \brief Constructor
    ///
    QLearning(const QLearningInput& input);

    ///
    /// \brief step. Performs the iterations for
    /// one training episode
    ///
    virtual void step()override final;

private:

    ///
    /// \brief Basic input
    ///
    QLearningInput input_;

    ///
    /// \brief episode_counter_ Counter for the episodes
    /// encountered
    ///
    uint_t episode_counter_;

    ///
    /// \brief actions_before_episodes_ Actions performed before starting
    /// the episodes
    ///
    virtual void actions_before_episodes_(){episode_counter_ = 0;}

    ///
    /// \brief actions_after_iterations_ Actions performed after finishing
    /// the iterations
    ///
    virtual void actions_after_iterations_();

};

template<typename WorldTp>
QLearning<WorldTp>::QLearning(const QLearningInput& input)
    :
    TDBase<WorldTp> (input.total_episodes),
    input_(input),
    episode_counter_(0)
{}


template<typename WorldTp>
void
QLearning<WorldTp>::step(){

    for(uint_t step=0; step < input_.max_num_iterations; ++step){

            // Will be used to obtain a seed for the random number engine
            std::random_device rd;

            // Standard mersenne_twister_engine seeded with rd()
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.0);
            auto exp_exp_tradeoff = dis(rd);

            // do exploration by default
            auto action_idx = this->world_ptr_->sample_action();

            if( exp_exp_tradeoff > input_.epsilon ){
                  action_idx = static_cast<action_t>(kernel::row_argmax(this->q_function_, this->state_->get_id()));
            }

            // step in the world
            auto [new_state, reward, finished, info] = this->world_ptr_->step(action_idx);

            // update the qtable
            this->q_function_(this->state_->get_id(), static_cast<uint_t>(action_idx)) += input_.learning_rate * (reward +
                                                           input_.discount_factor * kernel::get_row_max(this->q_function_, new_state->get_id()) -
                                                            this->q_function_(this->state_->get_id(), static_cast<uint_t>(action_idx)));

            this->state = new_state;

            if( finished ){
                break;
            }
        }
   }

template<typename WorldTp>
void
QLearning<WorldTp>::actions_after_iterations_(){
    episode_counter_ += 1;
    if(input_.use_decay){
        input_.epsilon = input_.min_epsilon + (input_.max_epsilon - input_.min_epsilon)*std::exp(-input_.epsilon_decay * episode_counter_);
    }
}

}
}
#endif
#endif // Q_LEARNING_H
