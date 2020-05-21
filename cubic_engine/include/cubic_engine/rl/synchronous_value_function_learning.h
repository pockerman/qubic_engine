#ifndef SYNCHRONOUS_VALUE_FUNCTION_LEARNER_H
#define SYNCHRONOUS_VALUE_FUNCTION_LEARNER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/utilities/iterative_algorithm_controller.h"

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
#include <vector>


namespace cengine {
namespace rl {

struct SyncValueFuncItrInput
{
    real_t tol;
    real_t gamma;
    uint_t n_iterations;
    bool show_iterations;
};

struct SyncValueFuncItrOutput
{
    real_t total_reward;
    real_t total_time;
};

/// \brief Synchronous value function iteration
/// learning implementation
template<typename WorldTp>
class SyncValueFuncItr: private boost::noncopyable
{
public:

    /// \brief The type of the world
    typedef WorldTp world_t;

    /// \brief The type of the action
    typedef typename world_t::action_t action_t;

    /// \brief The type of the reward
    typedef typename world_t::reward_value_t reward_value_t;

    /// \brief The type of the state
    typedef typename world_t::state_t state_t;

    /// \brief The input to initialize the algorithm
    typedef SyncValueFuncItrInput input_t;

    /// \brief The output type the train method returns
    typedef SyncValueFuncItrOutput output_t;

    /// \brief Constructor
    SyncValueFuncItr();

    /// \brief Constructor
    SyncValueFuncItr(SyncValueFuncItrInput&& input);


    /// \brief Train on the given world
    output_t train();

    ///Initialize the tabular implementation
    void initialize(world_t& world, real_t init_val);

private:

    /// \brief The input provided to the algorithm
    input_t imput_;

    /// \brief The object that controls the iterations
    kernel::IterativeAlgorithmController itr_controller_;

    /// \brief The old value function for each state
    std::vector<real_t> vold_;

    /// \brief The current value function for each state
    std::vector<real_t> v_;

    /// \brief Pointer to the world
    world_t* world_;


};

template<typename WorldTp>
typename SyncValueFuncItr<WorldTp>::output_t
SyncValueFuncItr<WorldTp>::train(){


    while(itr_controller_.continue_iterations()){

        if(imput_.show_iterations){
            std::cout<<itr_controller_.get_state()<<std::endl;
        }

        /// loop over the states of the world
        for(uint_t s=0; s<world_->n_states(); ++s){

            /// get the s-th state
            auto state = world_->get_state();

            /// the world should know which state is terminal
            if(!world_->is_goal_state(state)){

                /// this is not the goal state
                real_t old_v = vold_[state.get_id()];

                real_t weighted_sum = 0.0;

                /// loop over all the actions allowed on this
                /// state
                for(uint_t a=0; a<state.n_actions(); ++a){

                    auto action  = state.get_action(a);
                    auto state_prime = state.apply_action(a);

                    if(state_prime){

                        real_t p = world_->transition_dynamics(state_prime, state, action);
                        real_t reward = world_->get_reward(state, action);
                        real_t vs_prime = vold_[state_prime->get_id()];
                        weighted_sum += reward + p*imput_.gamma*vs_prime;
                    }

                }
            }
        }
    }

}



}

}

#endif // SYNCHRONOUS_VALUE_FUNCTION_LEARNER_H
