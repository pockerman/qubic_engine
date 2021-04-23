#ifndef TABULAR_SARSA_LEARNING_H
#define TABULAR_SARSA_LEARNING_H



#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/reward_table.h"
#include "cubic_engine/rl/td_base.h"
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
class Sarsa: public TDBase<WorldTp>
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
    typedef RLIterativeAlgoInput input_t;

    ///
    /// \brief The output type the train method returns
    ///
    typedef SarsaLearningOutput output_t;

    ///
    /// \brief Constructor
    ///
    Sarsa(const RLIterativeAlgoInput& input);

    ///
    /// \brief step. Performs the iterations for
    /// one training episode
    ///
    virtual void step()override final;

private:

};

template<typename WorldTp>
Sarsa<WorldTp>::Sarsa(const RLIterativeAlgoInput& input)
    :
    TDBase<WorldTp>(input)
{}


template<typename WorldTp>
void
Sarsa<WorldTp>::step(){

    action_t next_action = WorldTp::INVALID_ACTION;
    state_t* state = nullptr;

    auto action = this->action_selection_policy(*this->state_);

    auto& qtable = this->get_q_function();
    for(uint_t itr=1; itr < this->get_total_itrs_per_episode(); ++itr){

        // step in the world
        auto [next_state_ptr, reward, done, info] = this->world_ptr_->step(action);

        if(!next_state_ptr){
            throw std::logic_error("Next state pointer is NULL");
        }

        // Pick the next action
        next_action = this->action_selection_policy(*next_state_ptr);
        state = next_state_ptr;

        // TD Update
        auto td_target = reward + this->get_discount_factor() * qtable(next_state_ptr->get_id(), static_cast<uint_t>(next_action));
        auto td_delta = td_target - qtable(next_state_ptr->get_id(), static_cast<uint_t>(next_action));
        qtable(next_state_ptr->get_id(), static_cast<uint_t>(next_action)) += this->get_learning_rate()  * td_delta;

        if(done){
           break;
        }
    }
}

}
}

#endif // TABULAR_SARSA_LEARNING_H
