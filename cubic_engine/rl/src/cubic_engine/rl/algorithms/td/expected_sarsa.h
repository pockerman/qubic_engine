#ifndef EXPECTED_SARSA_H
#define EXPECTED_SARSA_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/rl/algorithms/td/td_algo_base.h"

#include "kernel/base/kernel_consts.h"
#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

namespace cengine {
namespace rl{
namespace algos {
namespace td {


///
/// \brief The  ExpectedSARSA class. Simple implementation
/// of the expected SARSA algorithm
///
template <typename TimeStepTp, typename ActionSelector>
class ExpectedSARSA: public  TDAlgoBase<TimeStepTp>
{
public:

    ///
    /// \brief env_t
    ///
    typedef typename TDAlgoBase<TimeStepTp>::env_t env_t;

    ///
    /// \brief action_t
    ///
    typedef typename TDAlgoBase<TimeStepTp>::action_t action_t;

    ///
    /// \brief state_t
    ///
    typedef typename TDAlgoBase<TimeStepTp>::state_t state_t;

    ///
    /// \brief action_selector_t
    ///
    typedef ActionSelector action_selector_t;

    ///
    /// \brief Constructor
    ///
    ExpectedSARSA(uint_t n_max_itrs, real_t tolerance,
                  real_t gamma, real_t eta, uint_t plot_f,
                  env_t& env, uint_t max_num_iterations_per_episode,
                  const ActionSelector& selector);

    ///
    /// \brief step. Performs the iterations for
    /// one training episode
    ///
    virtual void step()override final;

private:

    ///
    /// \brief action_selector_
    ///
    action_selector_t action_selector_;

    ///
    /// \brief update_q_table_
    /// \param action
    ///
    void update_q_table_(const action_t& action, const state_t& cstate,
                         const state_t& next_state, const  action_t& next_action, real_t reward);

};

template <typename TimeStepTp, typename ActionSelector>
ExpectedSARSA<TimeStepTp, ActionSelector>::ExpectedSARSA(uint_t n_max_itrs, real_t tolerance, real_t gamma,
                                         real_t eta, uint_t plot_f,
                                         env_t& env, uint_t max_num_iterations_per_episode, const ActionSelector& selector)
    :
      TDAlgoBase<TimeStepTp>(n_max_itrs, tolerance, gamma, eta, plot_f, max_num_iterations_per_episode, env),
      action_selector_(selector)
{}

template <typename TimeStepTp, typename ActionSelector>
void
ExpectedSARSA<TimeStepTp, ActionSelector>::step(){

    std::cout<<"Starting episode="<<this->current_iteration()<<std::endl;

    // total score for the episode
    auto score = 0.0;
    auto state = this->env_ref_().reset().observation();

    // select an action
    auto action = action_selector_(this->q_table(), state);

    uint_t itr=0;
    for(;  itr < this->max_num_iterations_per_episode(); ++itr){

        // select an action
        auto action = action_selector_(this->q_table(), state);
        if(this->is_verbose()){
            std::cout<<"Episode iteration="<<itr<<" of="<<this->actions_after_training_iterations()<<std::endl;
            std::cout<<"State="<<state<<std::endl;
            std::cout<<"Action="<<action<<std::endl;
        }

        // Take a step
        auto step_type_result = this->env_ref_().step(action);

        auto next_state = step_type_result.observation();
        auto reward = step_type_result.reward();
        auto done = step_type_result.done();

        // accumulate score
        score += reward;

        if(!done){
            auto next_action = action_selector_(this->q_table(), state);
            update_q_table_(action, state, next_state, next_action, reward);
            state = next_state;
            action = next_action;
        }
        else{

            update_q_table_(action, state, kernel::KernelConsts::invalid_size_type(),
                            kernel::KernelConsts::invalid_size_type(), reward);

            this->tmp_scores()[current_score_counter_++] = score;

            if(current_score_counter_ >= this->plot_frequency()){
                current_score_counter_ = 0;
            }

            if(this->is_verbose()){
                std::cout<<"============================================="<<std::endl;
                std::cout<<"Break out from episode="<<this->current_iteration()<<std::endl;
                std::cout<<"============================================="<<std::endl;
            }

            break;
        }
    }

    // make any adjustments to the way
    // actions are selected given the experience collected
    // in the episode
    action_selector_.adjust_on_episode(this->current_iteration());
    if(current_score_counter_ >= this->plot_frequency()){
        current_score_counter_ = 0;
    }

    std::cout<<"Finished step="<<this->current_iteration()<<std::endl;

}

template <typename TimeStepTp, typename ActionSelector>
void
ExpectedSARSA<TimeStepTp, ActionSelector>::update_q_table_(const action_t& action, const state_t& cstate,
                                                           const state_t& next_state, const  action_t& next_action, real_t reward){

#ifdef KERNEL_DEBUG
    assert(action < this->env_ref_().n_actions() && "Inavlid action idx");
    assert(cstate < this->env_ref_().n_states() && "Inavlid state idx");

    if(next_state != kernel::KernelConsts::invalid_size_type())
        assert(next_state < this->env_ref_().n_states() && "Inavlid next_state idx");

    if(next_action != kernel::KernelConsts::invalid_size_type())
        assert(next_action < this->env_ref_().n_actions() && "Inavlid next_action idx");
#endif

    const auto eps = action_selector_.eps_value();
    auto q_current = this->q_table()[cstate][action];
    auto policy_s = DynVec<real_t>(this->env_ref_().n_actions(), 1.0);
    policy_s *= eps / this->env_ref_().n_actions();

    auto state_action_values = this->q_table()[next_state];
    auto argmax = blaze::argmax(state_action_values);
    policy_s[argmax] = 1 - eps + (eps / this->env_ref_().n_actions());

    auto q_next = state_action_values * policy_s;
    auto td_target = reward + this->gamma() * q_next;
    this->q_table()[cstate][action] = q_current + (this->eta() * (td_target - q_current));
}

}
}
}
}

#endif // EXPECTED_SARSA_H
