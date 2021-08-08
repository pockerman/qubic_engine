#ifndef SARSA_H
#define SARSA_H

#include "cubic_engine/rl/algorithms/td/td_algo_base.h"

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
    Sarsa(uint_t n_max_itrs, real_t tolerance, real_t gamma, real_t eta, env_t& env);


    ///
    /// \brief step
    ///
    virtual void step()override final;

private:

    uint_t max_num_iterations_per_episode_;

    ///
    /// \brief action_selector_
    ///
    action_selector_t action_selector_;

    ///
    /// \brief update_q_table_
    /// \param action
    ///
    void update_q_table_(const action_t& action);
};

template <typename TimeStepTp, typename ActionSelector>
Sarsa<TimeStepTp, ActionSelector>::Sarsa(uint_t n_max_itrs, real_t tolerance, real_t gamma, real_t eta, env_t& env)
    :
      TDAlgoBase<TimeStepTp>(n_max_itrs, tolerance, gamma, eta, env)
{}


template <typename TimeStepTp, typename ActionSelector>
void
Sarsa<TimeStepTp, ActionSelector>::step(){


    auto score = 0.0;
    auto state = this->env_ref_().reset();

    // select an action
    auto action = action_selector_(this->q_table());

    for( uint_t itr=0;  itr < max_num_iterations_per_episode_; ++itr){

        // Take a step
        auto [prob, next_state, reward, done] = this->env_ref_().step(action);
        score += reward;

        if(not done){

            auto next_action = action_selector_(this->q_table());
            update_q_table_(next_action);
            state = next_state;
            action = next_action;
        }
        else{

            update_q_table_(next_action);
            this->tmp_scores().push_back(score);
            break;
        }
    }
}

}

}

}
}

#endif // SARSA_H
