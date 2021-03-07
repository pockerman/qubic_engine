#ifndef TABULAR_SARSA_LEARNING_H
#define TABULAR_SARSA_LEARNING_H


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
/// parameters for the  SarsaTableLearning class
///
struct SarsaLearningInput
{
    real_t learning_rate;
    real_t exploration_factor;
    real_t discount_factor;
    bool use_exploration;
    bool show_iterations;
};

///
/// \brief The SarsaLearningOutput struct
/// Helper struct to account for output of
/// SarsaTableLearning class
///
struct SarsaLearningOutput
{
    real_t total_reward;
    real_t total_time;
};

///
/// \brief Table-based implementation of SARSA algorithm
///
template<typename WorldTp>
class SarsaTableLearning: private boost::noncopyable
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

    /// \brief The type of the state
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
    SarsaTableLearning(SarsaLearningInput&& input);

    ///
    /// \brief Train on the given world
    ///
    output_t train(const state_t& goal );

    ///
    /// \brief Initialize the underlying data structures
    void initialize(world_t& world, reward_value_t val);

    ///
    /// \brief Returns the learnt tabular Qfunction
    ///
    const RewardTable<action_t, reward_value_t>& get_table()const{return qtable_;}

    ///
    /// \brief Returns the learnt tabular Qfunction
    ///
    RewardTable<action_t, reward_value_t>& get_table(){return qtable_;}

private:

    ///
    /// \brief Basic input
    ///
    SarsaLearningInput input_;

    ///
    /// \brief The QTable. A state is identified
    /// by an id. At each state a number of actions are
    /// possible. The score for each action is stored in
    /// a contiguous array
    ///
    RewardTable<action_t, reward_value_t> qtable_;

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
    /// \brief Initialize the QTable entries
    /// for the given state
    ///
    template<typename StateTp>
    void initialize_state_(const StateTp& state, reward_value_t val);

};

template<typename WorldTp>
SarsaTableLearning<WorldTp>::SarsaTableLearning(SarsaLearningInput&& input)
    :
    input_(std::move(input)),
    qtable_(),
    world_ptr_(nullptr),
    is_initialized_(false)
{}

template<typename WorldTp>
void
SarsaTableLearning<WorldTp>::initialize(typename SarsaTableLearning<WorldTp>::world_t& world,
                                    typename SarsaTableLearning<WorldTp>::reward_value_t val){


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
SarsaTableLearning<WorldTp>::initialize_state_(const StateTp& state,
                                           typename SarsaTableLearning<WorldTp>::reward_value_t val){

    for(uint_t a = 0; a<state.n_actions(); ++a){
        if(state.is_active_action(a)){
            qtable_.add_reward(state.get_id(), state.get_action_from_idx(a),  val);
        }
    }
}


template<typename WorldTp>
typename SarsaTableLearning<WorldTp>::output_t
SarsaTableLearning<WorldTp>::train(const typename SarsaTableLearning<WorldTp>::state_t& goal ){

    if(!is_initialized_){
        throw std::logic_error("SarsaTableLearning instance is not initialized");
    }

    auto start = std::chrono::steady_clock::now();
    typename SarsaTableLearning<WorldTp>::output_t output;
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
#endif // TABULAR_SARSA_LEARNING_H
