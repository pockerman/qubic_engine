#ifndef SARSA_H
#define SARSA_H

#include "cubic_engine/rl/algorithms/td/td_algo_base.h"
#include "kernel/base/kernel_consts.h"

namespace cengine{
namespace rl {
namespace algos {
namespace td {


template <typename TimeStepTp, typename ActionSelector>
class Sarsa: public TDAlgoBase<TimeStepTp>
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
    /// \brief Sarsa
    ///
    Sarsa(uint_t n_max_itrs, real_t tolerance,
          real_t gamma, real_t eta, env_t& env,
          uint_t max_num_iterations_per_episode, const ActionSelector& selector);

    ///
    /// \brief step
    ///
    virtual void step()override final;

private:

    ///
    /// \brief max_num_iterations_per_episode_
    ///
    uint_t max_num_iterations_per_episode_;

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
Sarsa<TimeStepTp, ActionSelector>::Sarsa(uint_t n_max_itrs, real_t tolerance, real_t gamma,
                                         real_t eta, env_t& env, uint_t max_num_iterations_per_episode, const ActionSelector& selector)
    :
      TDAlgoBase<TimeStepTp>(n_max_itrs, tolerance, gamma, eta, env),
      max_num_iterations_per_episode_(max_num_iterations_per_episode),
      action_selector_(selector)
{}


template <typename TimeStepTp, typename ActionSelector>
void
Sarsa<TimeStepTp, ActionSelector>::step(){


    auto score = 0.0;
    auto state = this->env_ref_().reset().observation();

    // select an action
    auto action = action_selector_(this->q_table());

    for( uint_t itr=0;  itr < max_num_iterations_per_episode_; ++itr){

        // Take a step
        auto step_type_result = this->env_ref_().step(action);

        auto next_state = step_type_result.observation();
        auto reward = step_type_result.reward();
        auto done = step_type_result.done();

        //auto [prob, next_state, reward, done] = this->env_ref_().step(action);
        score += reward;

        if(not done){

            auto next_action = action_selector_(this->q_table());
            update_q_table_(action, state, next_state, next_action, reward);
            state = next_state;
            action = next_action;
        }
        else{

            update_q_table_(action, state, kernel::KernelConsts::invalid_size_type(),
                            kernel::KernelConsts::invalid_size_type(), reward);

            this->tmp_scores().push_back(score);
            break;
        }
    }

    if(this->current_iteration() % this->plot_frequency()){
        this->avg_scores().push_back(0.0);
    }
}

template <typename TimeStepTp, typename ActionSelector>
void
Sarsa<TimeStepTp, ActionSelector>::update_q_table_(const action_t& action, const state_t& cstate,
                                                   const state_t& next_state, const action_t& next_action, real_t reward){

    auto q_current = this->q_table()[cstate][action];
    auto q_next = next_state != kernel::KernelConsts::invalid_size_type() ? this->q_table()[next_state][next_action] : 0.0;
    auto td_target = reward + this->gamma() * q_next;
    this->q_table()[cstate][action] = q_current + (this->eta() * (td_target - q_current));

}

}

}

}
}

#endif // SARSA_H
