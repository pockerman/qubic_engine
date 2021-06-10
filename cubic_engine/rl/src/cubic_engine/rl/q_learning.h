#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/td_base.h"
#include "cubic_engine/rl/rl_iterative_algo_input.h"
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
///
struct QLearningInput: public RLIterativeAlgoInput
{

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
    /// \brief Flag indicating if decaying is used
    ///
    bool use_decay{true};

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
    typedef typename TDBase<WorldTp>::world_t world_t;

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
    QLearningInput q_input_;

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
    TDBase<WorldTp> (input),
    q_input_(input),
    episode_counter_(0)
{}


template<typename WorldTp>
void
QLearning<WorldTp>::step(){



    for(uint_t step=0; step < this->get_total_itrs_per_episode(); ++step){

            // do exploration by default
            auto action_idx = this->action_selection_policy(this->state_);

            // step in the world
            auto [new_state, reward, finished, info] = this->world_ptr()->step(action_idx);

            // update the qtable
            this->q_function_(this->state_, static_cast<uint_t>(action_idx)) += this->get_learning_rate() * (reward +
                                                           this->get_discount_factor() * kernel::get_row_max(this->q_function_, new_state) -
                                                            this->q_function_(this->state_, static_cast<uint_t>(action_idx)));

            this->state_ = new_state;

            if( finished ){
                break;
            }
        }
   }

template<typename WorldTp>
void
QLearning<WorldTp>::actions_after_iterations_(){
    episode_counter_ += 1;
    if(q_input_.use_decay){

        auto epsilon = q_input_.min_epsilon + (q_input_.max_epsilon - q_input_.min_epsilon)*std::exp(-q_input_.epsilon_decay * episode_counter_);
        this->set_epsilon(epsilon);
    }
}

}
}

#endif // Q_LEARNING_H
