#include "cubic_engine/rl/policies/stochastic_adaptor_policy.h"
#include "cubic_engine/rl/policies/discrete_policy_base.h"
#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/utilities/array_utils.h"

#include "kernel/base/config.h"

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

namespace cengine {
namespace rl {
namespace policies {

StochasticAdaptorPolicy::StochasticAdaptorPolicy(uint_t state_space_size, uint_t action_space_size, std::shared_ptr<DiscretePolicyBase> policy)
    :
      DiscretePolicyAdaptorBase(),
      state_space_size_(state_space_size),
      action_space_size_(action_space_size),
      policy_(policy)
{}

StochasticAdaptorPolicy::~StochasticAdaptorPolicy()
{}

std::shared_ptr<DiscretePolicyBase>
StochasticAdaptorPolicy::operator()(const std::map<std::string, std::any>& options){

    auto state = std::any_cast<uint_t>(options.find("state")->second);
    auto state_actions = std::any_cast<DynVec<real_t>>(options.find("state_actions")->second);
    auto best_actions = kernel::utils::max_indices(state_actions);

#ifdef KERNEL_DEBUG
    assert(best_actions.size() <= action_space_size_ && "Incompatible number of best actions. Cannot exccedd the action space size");
#endif

    std::vector<std::pair<uint_t, real_t>> best_actions_vals(best_actions.size());

    for(uint_t i=0; i<best_actions.size(); ++i){
        best_actions_vals[i] = {best_actions[i], 1.0/best_actions.size()};
    }

    auto& state_action_vals = this->policy_->state_actions_values();

    auto& view = state_action_vals[state];

    //collect all the actions in a map
    auto act_val_map = std::unordered_map<uint_t, real_t>();

    for(uint_t a=0; a<best_actions_vals.size(); ++a){
        act_val_map.insert({best_actions_vals[a].first, best_actions_vals[a].second});
    }

    for(uint_t a=0; a<view.size(); ++a){
        auto action = view[a].first;

        if(act_val_map.contains(action)){
            view[a].second = act_val_map[action];
        }
        else{
            view[a].second = 0.0;
        }
    }

    return this->policy_;
}

}

}

}
