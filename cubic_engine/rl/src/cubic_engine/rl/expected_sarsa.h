#ifndef EXPECTED_SARSA_H
#define EXPECTED_SARSA_H

#include "cubic_engine/rl/td_base.h"
#include "kernel/maths/matrix_utilities.h"

namespace cengine {
namespace rl{

///
/// \brief The  ExpectedSARSA class. Simple implementation
/// of the expected SARSA algorithm
///
template<typename WorldTp>
class ExpectedSARSA: public TDBase<WorldTp>
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
    /// \brief Constructor
    ///
    ExpectedSARSA(const RLIterativeAlgoInput& input);

    ///
    /// \brief step. Performs the iterations for
    /// one training episode
    ///
    virtual void step()override final;

};

template<typename WorldTp>
void
ExpectedSARSA<WorldTp>::step(){

    action_t next_action = WorldTp::INVALID_ACTION;
    state_t* state = WorldTp::INVALID_STATE;

    // save the id of the old state
    auto old_state_id = this->state_->get_id();

    // choose an action based on the current state
    auto action = this->action_selection_policy(*this->state_);

    auto& qtable = this->get_q_function();
    for(uint_t itr=1; itr < this->get_total_itrs_per_episode(); ++itr){

        auto predict = qtable(this->state_->get_id(), action);

        // step in the world
        auto [next_state_ptr, reward, done, info] = this->world_ptr_->step(action);

        if(!next_state_ptr){
           throw std::logic_error("Next state pointer is NULL");
        }

        state = next_state_ptr;
        next_action = this->action_selection_policy(*next_state_ptr);

        auto expected_q = 0.0;
        auto q_max = kernel::get_row_max(qtable, state->get_id());
        auto greedy_actions = 0;

        for(uint_t a=0; a<this->world_ptr_->n_actions(); ++a){
            if(qtable(state->get_id(), a)){
                greedy_actions += 1;
            }
        }

        auto non_greedy_action_probability = this->get_epsilon() / this->world_ptr_->n_actions();
        auto greedy_action_probability = ((1 - this->get_epsilon()) / greedy_actions) + non_greedy_action_probability;

        for(uint_t i=0; i<this->world_ptr_->n_actions(); ++i) {

           if(qtable(state->get_id(), i) == q_max){
               expected_q += qtable(state->get_id(), i) * greedy_action_probability;
           }
           else{
               expected_q += qtable(state->get_id(), i) * non_greedy_action_probability;
           }
        }

        auto target = reward + this->get_discount_factor() * expected_q;
        qtable(old_state_id, action) += this->get_learning_rate() * (target - predict);

        if(done){
           break;
        }
    }


}

}
}

#endif // EXPECTED_SARSA_H
