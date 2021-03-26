#ifndef TABULAR_SARSA_LEARNING_H
#define TABULAR_SARSA_LEARNING_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/reward_table.h"
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
#include <chrono>

namespace cengine {
namespace rl {

///
/// \brief The SarsaLearningInput struct
/// Helper struct that collects all the
/// parameters for the  Sarsa class
///
struct SarsaLearningInput
{
    real_t learning_rate;
    real_t exploration_factor;
    real_t discount_factor;
    bool use_exploration;

    ///
    /// \brief show_iterations. Flag indicating if informative
    /// messages be printed during training
    ///
    bool show_iterations{true};

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
/// \brief The SarsaLearningOutput struct
/// Helper struct to account for output of
/// Sarsa class
///
struct SarsaLearningOutput
{
    real_t total_reward;
    real_t total_time;
};

///
/// \brief SARSA algorithm: On-policy TD control.
///  Finds the optimal epsilon-greedy policy.
///
template<typename WorldTp>
class Sarsa: private boost::noncopyable
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
    /// \brief The input to initialize the algorithm
    ///
    typedef SarsaLearningInput input_t;

    ///
    /// \brief The output type the train method returns
    ///
    typedef SarsaLearningOutput output_t;

    ///
    /// \brief Constructor
    ///
    Sarsa(SarsaLearningInput&& input);

    ///
    /// \brief Train on the given world
    ///
    output_t train();

    ///
    /// \brief step. Performs the iterations for
    /// one training episode
    ///
    void step(const action_t& action);

    ///
    /// \brief Initialize the underlying data structures
    ///
    void initialize(world_t& world, reward_value_t val);

    ///
    /// \brief Returns the learnt Qfunction
    ///
    const DynMat<real_t>& get_q_function()const{return qtable_;}

    ///
    /// \brief Returns the learnt Qfunction
    ///
    DynMat<real_t>& get_q_function(){return qtable_;}

private:

    ///
    /// \brief Basic input
    ///
    SarsaLearningInput input_;

    ///
    /// \brief qtable_. The tabe that represents the state-action_value
    /// pairs for each action
    ///
    DynMat<real_t> qtable_;

    ///
    /// \brief The world used by the agent
    ///
    world_t* world_ptr_;

    ///
    /// \brief Flag indicating if the trainer
    /// has been initialized
    ///
    bool is_initialized_;

    ///
    /// \brief Apply epsilon greedy strategy for action selection
    ///
    action_t epsilon_greedy_(const state_t& state);

};

template<typename WorldTp>
Sarsa<WorldTp>::Sarsa(SarsaLearningInput&& input)
    :
    input_(std::move(input)),
    qtable_(),
    world_ptr_(nullptr),
    is_initialized_(false)
{}

template<typename WorldTp>
void
Sarsa<WorldTp>::initialize(typename Sarsa<WorldTp>::world_t& world,
                                    typename Sarsa<WorldTp>::reward_value_t val){

    // clear any entries that we may have
    qtable_.clear();
    qtable_.resize(world.n_states(), world.n_actions());

    // loop over the states of the world and initialize
    // the action scores
    for(uint_t s=0; s<world.n_states(); ++s){
        for(uint_t a=0; a < world.n_actions(); ++a)
        qtable_(s, a) = val;
    }

    // finally set the world pointer
    world_ptr_ = &world;
    is_initialized_ = true;
}

/*template<typename WorldTp>
template<typename StateTp>
void
Sarsa<WorldTp>::initialize_state_(const StateTp& state,
                                           typename Sarsa<WorldTp>::reward_value_t val){

    for(uint_t a = 0; a<state.n_actions(); ++a){
        if(state.is_active_action(a)){
            qtable_.add_reward(state.get_id(), state.get_action_from_idx(a),  val);
        }
    }
}*/

template<typename WorldTp>
void
Sarsa<WorldTp>::step(const action_t& action){

    action_t next_action = WorldTp::INVALID_ACTION;
    state_t* state = WorldTp::INVALID_STATE;

    for(uint_t itr=1; itr < input_.max_num_iterations; ++itr){

        // step in the world
        auto [next_state_ptr, reward, done, info] = world_ptr_->step(action);

        if(!next_state_ptr){
            throw std::logic_error("Next state pointer is NULL");
        }

        // Pick the next action
        next_action = epsilon_greedy_(*next_state_ptr);
        state = next_state_ptr;

        // TD Update
        auto td_target = reward + input_.discount_factor * qtable_(next_state_ptr->get_id(), next_action);
        auto td_delta = td_target - qtable_(next_state_ptr->get_id(), next_action);
        qtable_(next_state_ptr->get_id(), next_action) += input_.learning_rate  * td_delta;

        if(done){
           break;
        }
    }
}

template<typename WorldTp>
typename Sarsa<WorldTp>::output_t
Sarsa<WorldTp>::train(){

    if(!is_initialized_){
        throw std::logic_error("QLearning instance is not initialized");
    }

    auto start = std::chrono::steady_clock::now();
    typename Sarsa<WorldTp>::output_t output;
    output.total_reward = 0.0;
    output.total_time = 0.0;



    auto action_idx = WorldTp::invalid_action;

    uint_t itr_counter = 0;
    while( world_ptr_->get_current_state() != goal && !world_ptr_->is_finished()){

        // get the current state of the world
        auto& state = world_ptr_->get_current_state();

        if(input_.show_iterations){
            std::cout<<"\tAt iteration: "<<itr_counter +1 <<std::endl;
            std::cout<<"\tCurrent state: "<<state.get_id()<<std::endl;
        }

        // we just started
        if(action_idx == WorldTp::invalid_action){
            action_idx = qtable_.get_max_reward_action_at_state(state.get_id());

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

            // replace with random action to
            // promote exploration
            if(input_.use_exploration && r < input_.exploration_factor){

                // Will be used to obtain a seed for the random number engine
                std::random_device rd;

                // Standard mersenne_twister_engine seeded with rd()
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, state.n_actions()-1);
                auto idx = dis(rd);

                action_idx = state.get_action_from_idx(idx);
            }
        }

        if(input_.show_iterations){
            std::cout<<"\tTaking action: "<<worlds::to_string(action_idx)<<std::endl;
        }

        // take action
        world_ptr_->step(action_idx);

        if(world_ptr_->is_finished()){

            auto reward = world_ptr_->reward();
            output.total_reward += reward;

            // the action led to a catastrophy according to the world
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

            // observe R and S'
            auto reward = world_ptr_->reward();

            std::cout<<"\tReward received: "<<reward<<std::endl;

            output.total_reward += reward;

            auto& new_state = world_ptr_->get_current_state();
            auto current_val = qtable_.get_reward(state.get_id(), action_idx);

            std::cout<<"\tCurrent value for state: "<<state.get_id()
                    <<" and action: "<<worlds::to_string(action_idx)
                   <<" "<<current_val<<std::endl;

            // choose A' from S'
            auto next_action_idx = qtable_.get_max_reward_action_at_state(new_state.get_id());

            if(input_.show_iterations){
                std::cout<<"\tNext action: "<<worlds::to_string(next_action_idx)<<std::endl;
            }

            if(input_.use_exploration){

                // Will be used to obtain a seed for the random number engine
                std::random_device newrd;

                ///Standard mersenne_twister_engine seeded with rd()
                std::mt19937 newgen(newrd());
                std::uniform_real_distribution<> newdis(0.0, 1.0);
                auto epsilon = newdis(newrd);

                if(epsilon < input_.exploration_factor){
                    next_action_idx = new_state.get_random_active_action();
                    std::cout<<"\tRecalculated next action: "<<worlds::to_string(next_action_idx)<<std::endl;
                }
            }

            if(input_.show_iterations){
                std::cout<<"\tNext state: "<<new_state.get_id()<<std::endl;
            }

            auto future_reward = qtable_.get_reward(new_state.get_id(), next_action_idx);

            auto val = current_val + input_.learning_rate*(reward +
                                         input_.discount_factor * future_reward - current_val);

            if(input_.show_iterations){
                std::cout<<"\tSetting for state: "<<state.get_id()
                        <<" and action: "<<worlds::to_string(action_idx)
                        <<" to value: "<<val<<std::endl;
            }
            qtable_.set_reward(state.get_id(), action_idx, val );

            action_idx = next_action_idx;
            itr_counter++;
        }
      }//end of while

     auto end = std::chrono::steady_clock::now();
     std::chrono::duration<double> elapsed_seconds = end-start;
     output.total_time = elapsed_seconds.count();

     return output;
}

}
}
#endif
#endif // TABULAR_SARSA_LEARNING_H
