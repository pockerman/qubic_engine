#ifndef POLICY_IMPROVEMENT_H
#define POLICY_IMPROVEMENT_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/iterative_algorithm_controller.h"
#include "kernel/utilities/iterative_algorithm_result.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdexcept>

namespace cengine {
namespace rl {

///
/// \brief The PolicyIterationInput struct
/// Helper for assembling the input for the PolicyIterationInput class
///
struct PolicyIterationInput
{
    real_t tol;
    real_t gamma;
    uint_t n_iterations;
    bool show_iterations;
};

///
/// \brief The PolicyIterationOutput struct
/// Helper for summarizing the output when the
/// PolicyIterationOutput::train function is called
///
struct PolicyIterationOutput
{
    real_t total_reward;
    std::chrono::seconds total_time;
};


///
/// \brief The PolicyIteration class. Policy iteration
/// with policy evaluation
///
template<typename WorldTp>
class PolicyIteration
{
public:

    ///
    /// \brief The input to initialize the algorithm
    ///
    typedef PolicyIterationInput input_t;

    ///
    /// \brief The output type the train method returns
    ///
    typedef kernel::IterativeAlgorithmResult output_t;

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
    /// \brief PolicyIteration. Constructor
    ///
    PolicyIteration(const input_t& in);

    ///
    /// \brief Train on the given world using the given policy
    /// and the given dynamics function
    ///
    template<typename PolicyTp, typename PolicyEvaluatorTp>
    output_t train(PolicyTp& policy, PolicyEvaluatorTp& policy_evaluator);

    ///
    /// \brief Performs one step of the training on the given world
    ///
    template<typename PolicyTp, typename PolicyEvaluatorTp>
    void step(PolicyTp& policy, PolicyEvaluatorTp& policy_evaluator);

    ///
    /// \brief Initialize the underlying data structures
    ///
    void initialize(world_t& world, real_t init_val);

    ///
    /// \brief Returns true if iterations should be continued
    ///
    bool continue_iterations(){return itr_controller_.continue_iterations();}

    ///
    /// \brief Returns  the current iteration index
    ///
    uint_t get_current_iteration()const{return itr_controller_.get_current_iteration();}

private:

    ///
    /// \brief The object that controls the iterations
    ///
    kernel::IterativeAlgorithmController itr_controller_;

    ///
    /// \brief input_. Input to the algorithm
    ///
    input_t input_;

    ///
    /// \brief Pointer to the world
    ///
    world_t* world_;

    ///
    /// \brief one_step_lookahed_. Helper function to calculate the value
    /// for all action in a given state.
    ///
    DynVec<real_t> one_step_lookahed_(const state_t& state, const DynVec<real_t>& value_func_table)const;
};

template<typename WorldTp>
PolicyIteration<WorldTp>::PolicyIteration(const input_t& in)
    :
      itr_controller_(in.n_iterations, in.tol),
      input_(in)
{}

template<typename WorldTp>
template<typename PolicyTp, typename PolicyEvaluatorTp>
void
PolicyIteration<WorldTp>::step(PolicyTp& policy, PolicyEvaluatorTp& policy_evaluator){

   // Evaluate the current policy
   auto value_func_table = policy_evaluator(*world_, policy, input_.gamma, input_.tol);

   // Will be set to false if we make any changes to the policy
   auto policy_stable = true;

   // For each state...
   for(uint_t s=0; s<world_->n_states(); ++s){

       // The best action we would take under the current policy
       auto chosen_a = policy.get_best_policy_at_state(world_->get_state(s));

       // Find the best action by one-step lookahead
       // Ties are resolved arbitarily
       auto action_values = one_step_lookahead_(s, value_func_table);
       auto best_a = blaze::max(action_values);

       // Greedily update the policy
       if( chosen_a != best_a){
           policy_stable = false;
       }

       throw std::logic_error("Policy for state not updated");
       //self._policy[s] = np.eye(self._env.nA)[best_a]

   }

   // If the policy is stable we've found an optimal policy.
   // signal convergence
   if(policy_stable){
       itr_controller_.update_residual( 0. );
   }

}


template<typename WorldTp>
template<typename PolicyTp, typename PolicyEvaluatorTp>
typename PolicyIteration<WorldTp>::output_t
PolicyIteration<WorldTp>::train(PolicyTp& policy, PolicyEvaluatorTp& policy_evaluator){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    while(itr_controller_.continue_iterations()){

        if(input_.show_iterations){
            std::cout<<itr_controller_.get_state()<<std::endl;
        }
        step(policy, policy_evaluator);
    }

    auto state = itr_controller_.get_state();

    end = std::chrono::system_clock::now();
    state.total_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    return state;
}

template<typename WorldTp>
DynVec<real_t>
PolicyIteration<WorldTp>::one_step_lookahed_(const state_t& state,  const DynVec<real_t>& value_func_table)const{

    DynVec<real_t> values(state.n_actions(), 0.0);

    for(uint_t a=0; a<state.n_actions(); ++a){
        for(uint_t s=0; s<state.n_states(); ++s){

            auto& trans_state = *state.get_states()[s];
            auto action = state.get_action(a);
            auto prob = world_->get_dynamics(trans_state, action);
            auto reward = world_->get_reward(state, action);
            auto vs_prime = value_func_table[trans_state.get_id()];
            values[a] += prob * (reward + input_.gamma * vs_prime );
        }
    }

    return values;
}


}
}

#endif // POLICY_IMPROVEMENT_H
