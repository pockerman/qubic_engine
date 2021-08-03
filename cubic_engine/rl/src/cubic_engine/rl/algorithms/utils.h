#ifndef UTILS_H
#define UTILS_H

#include "cubic_engine/base/cubic_engine_types.h"
#include <vector>

namespace cengine{
namespace rl {
namespace algos {

///
/// Given the state index returns the list of actions under the
/// provided value functions
///
template<typename WorldTp>
auto state_actions_from_v(const WorldTp& env, const DynVec<real_t>& v,
                          real_t gamma, uint_t state) -> DynVec<real_t>{

    auto q = DynVec<real_t>(env.n_actions(), 0.0);

    for(uint_t a=0; a < env.n_actions(); ++a){

        const auto& transition_dyn = env.transition_dynamics(state, a);

        for(auto& dyn: transition_dyn){
            auto prob = std::get<0>(dyn); //.probabilities[0];
            auto next_state = std::get<1>(dyn); //.states[0];
            auto reward = std::get<2>(dyn); //.rewards[0];
            auto done = std::get<3>(dyn); //.dones[0];

            q[a] += prob * (reward + gamma * v[next_state]);

        }

    }

    return q;
}

}
}
}

#endif // UTILS_H
