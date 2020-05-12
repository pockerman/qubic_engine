#ifndef SYNCHRONOUS_VALUE_FUNCTION_LEARNER_H
#define SYNCHRONOUS_VALUE_FUNCTION_LEARNER_H

#include "cubic_engine/base/cubic_engine_types.h"
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
#include <vector>


namespace cengine {
namespace rl {

struct SyncValueFuncItrInput
{
    real_t discount_factor;
    bool show_iterations;
};

struct SyncValueFuncItrOutput
{
    real_t total_reward;
    real_t total_time;
};

/// \brief Synchronous value function iteration
/// learning implementation
///
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

    /// \brief Train on the given world
    output_t train(const state_t& goal );

    ///Initialize the tabular implementation
    void initialize(world_t& world, real_t init_val);



private:

    /// \brief The old value function for each state
    std::vector<real_t> vold_;

    /// \brief The current value function for each state
    std::vector<real_t> v_;


};



}

}

#endif // SYNCHRONOUS_VALUE_FUNCTION_LEARNER_H
