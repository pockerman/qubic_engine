#ifndef POLICY_EVALUATOR_H
#define POLICY_EVALUATOR_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine{
namespace rl{

///
/// \brief The PolicyEvaluator class
///
/// Evaluate a policy given an environment and a full
/// description of the environment's dynamics.
///
template<typename WorldTp, typename PolicyTp>
class PolicyEvaluator
{

public:
    ///
    /// \brief The type of the world
    ///
    typedef WorldTp world_t;

    ///
    /// \brief The type of the policy
    ///
    typedef PolicyTp policy_t;

    ///
    /// \brief operator () Evaluate the policy. Returns
    /// a vector of length world.n_states( representing the value function.
    ///
    DynVec<real_t> operator()(const world_t& world, const policy_t& policy, real_t discount_factor, real_t tol);

};

template<typename WorldTp, typename PolicyTp>
DynVec<real_t>
PolicyEvaluator<WorldTp, PolicyTp>::operator()(const world_t& world, const policy_t& policy, real_t discount_factor, real_t tol){

    // Start with a random (all 0) value function
    DynVec<real_t> v(world.n_states(), 0.0);
    while ( true){

        auto delta = 0.;

        // For each state, perform a "full backup"
        for(uint_t s=0; s<world.n_states(); ++s){

            auto vtmp = 0.0;

            // get the s-th state
            auto& state = world.get_state(s);

            // actions allowed in this state
            auto actions = state.get_actions();

            // look at the possible next actions
            for(uint_t a=0; a<actions.size(); ++a){

                // the probability of taking action a
                // when at state s
                auto action_prob = policy.get_action_probability(state, actions[a]);

                // For each action, look at the possible next states...
                auto prob = world.get_dynamics(state, actions[a]);

                // get the state we transition when performing
                // action a
                auto& trans_state = *state.get_state(actions[a]);

                auto reward = world.get_reward(state, actions[a]);
                vtmp += action_prob * prob * (reward + discount_factor * v[trans_state.get_id()]);

            }

            delta = std::max(delta, std::fabs((vtmp, v[s])));
            v[s] = vtmp;
        }

        if( delta < tol){
            break;
        }
       }

       return v;

}
}
}

#endif
#endif // POLICY_EVALUATOR_H
