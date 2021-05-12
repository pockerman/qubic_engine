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
#include <algorithm>
#include <cmath>
#include <tuple>


namespace cengine {
namespace rl {

///
/// \brief The SyncValueFuncItrInput struct
/// Helper for assembling the input for the SyncValueFuncItr class
///
struct SyncValueFuncItrInput
{
    real_t tol;
    real_t gamma;
    uint_t n_iterations;
    bool show_iterations;
};

///
/// \brief The SyncValueFuncItrOutput struct
/// Helper for summarizing the output when the
/// SyncValueFuncItr::train function is called
///
struct SyncValueFuncItrOutput
{
    real_t total_reward;
    real_t total_time;
};

///
/// \brief The SyncValueFuncItr class. Models
/// the iterative policy evaluation algorithm for learning
/// a value function \f$V\f$ under a policy \f$\pi\f$. The  implementation
/// uses a two array approach. Thus it is assumed the world,
/// action and reward spaces are finite. The WorldTp template parameter
/// should follow the interface of the DiscreteWorld class
///
template<typename WorldTp>
class SyncValueFuncItr: private boost::noncopyable
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
    typedef SyncValueFuncItrInput input_t;

    ///
    /// \brief The output type the train method returns
    ///
    typedef void output_t;

    ///
    /// \brief Default constructor
    ///
    SyncValueFuncItr();

    ///
    /// \brief Constructor
    ///
    SyncValueFuncItr(const SyncValueFuncItrInput& input);

    ///
    /// \brief Train on the given world using the given policy
    /// and the given dynamics function
    ///
    output_t train();

    ///
    /// \brief Performs one step of the training on the given world
    ///
    void step();

    ///
    /// \brief Initialize the underlying data structures
    ///
    void initialize(world_t& world, real_t init_val);

    ///
    /// \brief Returns true if iterations should be continued
    ///
    bool continue_iterations(){return itr_controller_.continue_iterations();}

    ///
    /// \brief Access the value function table
    ///
    const std::vector<real_t>& get_values()const{return v_;}

    ///
    /// \brief Returns  the current iteration index
    ///
    uint_t get_current_iteration()const{return itr_controller_.get_current_iteration();}

private:

    ///
    /// \brief The input provided to the algorithm
    ///
    input_t input_;

    ///
    /// \brief The object that controls the iterations
    ///
    kernel::IterativeAlgorithmController itr_controller_;

    ///
    /// \brief The old value function for each state
    ///
    std::vector<real_t> vold_;

    ///
    /// \brief The current value function for each state
    ///
    std::vector<real_t> v_;

    ///
    /// \brief Pointer to the world
    ///
    world_t* world_;

    ///
    /// \brief one_step_lookahed_. Helper function to calculate the value
    /// for all action in a given state.
    ///
    DynVec<real_t> one_step_lookahed_(const state_t& state)const;
};

template<typename WorldTp>
SyncValueFuncItr<WorldTp>::SyncValueFuncItr()
    :
    input_(),
    itr_controller_(0, 1.0e-8),
    vold_(),
    v_(),
    world_(nullptr)
{}

template<typename WorldTp>
SyncValueFuncItr<WorldTp>::SyncValueFuncItr(const SyncValueFuncItrInput& input)
    :
    input_(input),
    itr_controller_(input.n_iterations, input.tol),
    vold_(),
    v_(),
    world_(nullptr)
{}

template<typename WorldTp>
void
SyncValueFuncItr<WorldTp>::initialize(world_t& world, real_t init_val){

    world_ = &world;
    vold_.resize(world_->n_states(), init_val);
    v_.resize(world_->n_states(), init_val);
}

template<typename WorldTp>
void
SyncValueFuncItr<WorldTp>::step(){

    if(world_ == nullptr){
        std::runtime_error("World pointer is null.");
    }

    real_t delta = 0.0;

    // loop over the states of the world
    for(uint_t s=0; s<world_->n_states(); ++s){

        // Do a one-step lookahead to find the best action
        // look ahead values for the state
        auto look_ahead_vals = one_step_lookahed_(s);
        auto best_action_val = blaze::max(look_ahead_vals);
        auto old_v = vold_[s];
        delta = std::max(delta, std::fabs(old_v - best_action_val));
        v_[s] = best_action_val;
    }

    // update the residual of the controller
    itr_controller_.update_residual(delta);

    // finally update the vectors
    vold_ = v_;
}

template<typename WorldTp>
typename SyncValueFuncItr<WorldTp>::output_t
SyncValueFuncItr<WorldTp>::train(){

    while(itr_controller_.continue_iterations()){

        if(input_.show_iterations){
            std::cout<<itr_controller_.get_state()<<std::endl;
        }
        step();
    }
}

template<typename WorldTp>
DynVec<real_t>
SyncValueFuncItr<WorldTp>::one_step_lookahed_(const state_t& state)const{

    const static auto n_actions = world_->n_actions();
    DynVec<real_t> values(world_->n_actions(), 0.0);

    for(uint_t a=0; a<n_actions; ++a){

        auto dynamics = world_->get_dynamics(state, a);

        for(uint_t item=0; item<dynamics.size(); ++item){

            auto prob = std::get<0>(dynamics[item]);
            auto next_state = std::get<1>(dynamics[item]);
            auto r = std::get<2>(dynamics[item]);
            auto vs_prime = vold_[next_state];
            values[a] += prob*(r + input_.gamma*vs_prime );
        }
    }

    return values;
}

}
}

#endif // SYNCHRONOUS_VALUE_FUNCTION_LEARNER_H
