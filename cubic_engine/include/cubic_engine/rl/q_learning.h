#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/action_state_function_table.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/maths/matrix_utilities.h"

#if defined(__GNUC__) && (__GNUC___ > 7)
#include "magic_enum.hpp"
#else
#include "cubic_engine/rl/worlds/grid_world_action_space.h"
#endif
#include <boost/noncopyable.hpp>
#include <string>
#include <map>
#include <random>
#include <iostream>
#include <cmath>

namespace cengine {
namespace rl{

///
/// \brief The QLearningInput struct
/// Helper struct that assembles the input for the QTableLearning class.
struct QLearningInput
{
    real_t learning_rate{0.01};
    real_t epsilon{1.0};
    real_t max_epsilon{1.0};
    real_t min_epsilon{0.01};
    real_t epsilon_decay{0.01};
    real_t discount_factor{0.6};
    bool use_exploration;
    bool show_iterations{true};
    bool use_decay{true};
    uint_t max_num_iterations;
    uint_t total_episodes;
};


///
/// \brief The QTableLearning class. Table based implementation
/// of the Q-learning algorithm using epsilon-greedy policy.
template<typename WorldTp>
class QTableLearning: private boost::noncopyable
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
    QTableLearning(QLearningInput&& input);

    ///
    /// \brief Train on the given world
    ///
    void train();

    ///
    /// \brief Initialize the underlying data structures
    ///
    void initialize(world_t& world, reward_value_t val);

    ///
    /// \brief Returns the learnt Qfunction
    ///
    const RewardTable<action_t, reward_value_t>& get_table()const{return qtable_;}

    ///
    /// \brief Returns the learnt Qfunction
    ///
    RewardTable<action_t, reward_value_t>& get_table(){return qtable_;}

private:

    ///
    /// \brief Basic input
    ///
    QLearningInput input_;

    ///
    /// \brief The QTable. A state is identified
    /// by an id. At each state a number of actions are
    /// possible. The score for each action is stored in
    /// a contiguous array.
    ///
    RewardTable<action_t, reward_value_t> qtable_;

    ///
    /// \brief q_function_
    ///
    DynMat<real_t> q_function_;

    ///
    /// \brief The world used by the agent
    ///
    world_t* world_ptr_;

    ///
    /// \brief Flag indicating if the trainer has been initialized
    ///
    bool is_initialized_;

    ///
    /// \brief Initialize the QTable entries for the given state
    ///
    template<typename StateTp>
    void initialize_state_(const StateTp& state, reward_value_t val);

    ///
    /// \brief make_epsilon_greedy_greedy
    ///
    void make_epsilon_greedy_greedy();
};

template<typename WorldTp>
QTableLearning<WorldTp>::QTableLearning(QLearningInput&& input)
    :
    input_(std::move(input)),
    qtable_(),
    world_ptr_(nullptr),
    is_initialized_(false)
{}

template<typename WorldTp>
void
QTableLearning<WorldTp>::initialize(world_t& world, reward_value_t val){

    // loop over the states of the world and initialize
    // the action scores
    for(uint_t s=0; s<world.n_states(); ++s){
        initialize_state_(world.get_state(s), val);
    }


    // finally set the world pointer
    world_ptr_ = &world;
    is_initialized_ = true;
}

template<typename WorldTp>
template<typename StateTp>
void
QTableLearning<WorldTp>::initialize_state_(const StateTp& state, reward_value_t val){

    for(uint_t a = 0; a<state.n_actions(); ++a){
        if(state.is_active_action(a)){
            qtable_.add_reward(state.get_id(), state.get_action_from_idx(a),  val);
        }
    }
}


template<typename WorldTp>
void
QTableLearning<WorldTp>::train(){

    if(!is_initialized_){
        throw std::logic_error("QTableLearning instance is not initialized");
    }

    // the policy we are following
    auto policy = make_epsilon_greedy_greedy();

    uint_t episode_counter = 0;
    for(uint_t itr=0; itr < input_.total_episodes; ++itr){

        // for every iteration reset the environment
        auto& state = world_ptr_->restart();

        for(uint_t step=0; step < input_.max_num_iterations; ++step){

            // Will be used to obtain a seed for the random number engine
            std::random_device rd;

            // Standard mersenne_twister_engine seeded with rd()
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.1);
            auto exp_exp_tradeoff = dis(rd);

            // do exploration by default
            auto action_idx = world_ptr_->sample_action();

            if( exp_exp_tradeoff > input_.epsilon ){
                  action_idx = kernel::row_argmax(q_function_, state.get_id());
            }

            // step in the world
            auto [new_state, reward, finished, info] = world_ptr_->step(action_idx);

            // update the qtable
            q_function_(state.get_id(), action_idx) += input_.learning_rate * (reward +
                                                           input_.discount_factor * kernel::get_row_max(q_function_, new_state.get_id()) -
                                                            q_function_(state.get_id(), action_idx));

            state = new_state;

            if( finished ){
                break;
            }
        }

        if(input_.use_decay){
            episode_counter += 1;
            input_.epsilon = input_.min_epsilon + (input_.max_epsilon - input_.min_epsilon)*std::exp(-input_.epsilon_decay * episode_counter);
        }
    }
}

}

}
#endif
#endif // Q_LEARNING_H
