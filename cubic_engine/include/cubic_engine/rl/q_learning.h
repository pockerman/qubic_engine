#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/action_state_function_table.h"
#include "kernel/base/kernel_consts.h"

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

namespace cengine {
namespace rl{

///
/// \brief The QLearningInput struct
/// Helper struct that assembles the input for the QTableLearning class.
struct QLearningInput
{
    real_t learning_rate;
    real_t epsilon;
    real_t discount_factor;
    bool use_exploration;
    bool show_iterations;
    uint_t max_num_iterations;
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
    ActionStateFunctionTable q_function_;

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

    for(uint_t itr=0; itr < input_.max_num_iterations; ++itr){

        // for every iteration reset the environment

        while(true){

            // action probabilities
            auto action_probs = policy(state);
            [next_state, reward, done] = world_ptr_->step();

            // TD Update
            auto best_next_action = np.argmax(Q[next_state])
            td_target = reward + discount_factor * Q[next_state][best_next_action]
            td_delta = td_target - Q[state][action]
            Q[state][action] += alpha * td_delta
        }
    }

    auto goal = world_ptr_->get_goal();

    uint_t itr_counter = 0;
    while( world_ptr_->get_current_state() != goal && !world_ptr_->is_finished()){

        // get the current state of the world
        auto& state = world_ptr_->get_current_state();

        if(input_.show_iterations){
            std::cout<<"At iteration: "<<itr_counter +1 <<std::endl;
            std::cout<<"\tCurrent state: "<<state.get_id()<<std::endl;
        }

        auto action_idx = qtable_.get_max_reward_action_at_state(state.get_id());

        // generate a random number
        // between 0 and 1
        auto r = 0.0;
        if(input_.use_exploration){

            // Will be used to obtain a seed for the random number engine
            std::random_device rd;

            // Standard mersenne_twister_engine seeded with rd()
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.1);
            r = dis(rd);
        }

        // replace with random action to promote exploration
        if(input_.use_exploration && r < input_.exploration_factor){

            // Will be used to obtain a seed for the random number engine
            std::random_device rd;

            // Standard mersenne_twister_engine seeded with rd()
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, state.n_actions()-1);
            auto idx = dis(rd);

            action_idx = state.get_action_from_idx(idx);
        }


        world_ptr_->step(action_idx);
        if(world_ptr_->is_finished()){

            // the action led to a catastrophy according to
            // the world
            if(input_.show_iterations){

#if defined(__GNUC__) && (__GNUC___ > 7)
                std::cout<<"WORLD FINISHED AT STATE: "<<state.get_id()
                        <<" AND ACTION: "<<magic_enum::enum_name(action_idx)<<std::endl;
#else
                std::cout<<"WORLD FINISHED AT STATE: "<<state.get_id()
                        <<" AND ACTION: "<<worlds::to_string(action_idx)<<std::endl;
#endif

            }

            break;
        }
        else{


            // get the reward
            auto reward = world_ptr_->reward();
            auto current_val = qtable_.get_reward(state.get_id(), action_idx);

            // update the table
            auto& new_state = world_ptr_->get_current_state();

            auto future_reward = qtable_.get_max_reward_at_state(new_state.get_id());

            auto val = current_val + input_.learning_rate*(reward +
                                         input_.discount_factor * future_reward - current_val);

            if(input_.show_iterations){
                std::cout<<"\t Setting for state: "<<state.get_id()
                        <<" and action: "<<worlds::to_string(action_idx)
                        <<" to value: "<<val<<std::endl;
            }
            qtable_.set_reward(state.get_id(), action_idx, val );

            itr_counter++;
        }
      }
}

}

}
#endif
#endif // Q_LEARNING_H
