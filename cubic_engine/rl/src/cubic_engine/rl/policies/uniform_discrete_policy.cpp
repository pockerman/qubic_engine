#include "cubic_engine/rl/policies/uniform_discrete_policy.h"
#include "cubic_engine/rl/policies/policy_type.h"

namespace cengine{
namespace rl {
namespace policies {

namespace  {

bool operator == (const std::pair<uint_t, uint_t>& s1, const std::pair<uint_t, uint_t>& s2){

    return s1.first == s2.first && s1.second == s2.second;
}

bool operator != (const std::pair<uint_t, uint_t>& s1, const std::pair<uint_t, uint_t>& s2){

    return !(s1 == s2);
}

}


UniformDiscretePolicy::UniformDiscretePolicy(uint_t n_states, uint_t n_actions)
    :
      DiscretePolicyBase(PolicyType::DISCRETE_UNIFORM),
      n_states_(n_states),
      n_actions_(n_actions),
      val_(-1.0),
      state_actions_prob_()
{
    val_ = 1.0/static_cast<real_t>(n_actions_);
    init_();
}


UniformDiscretePolicy::UniformDiscretePolicy(uint_t n_states, uint_t n_actions, real_t val)
    :
     DiscretePolicyBase(PolicyType::DISCRETE_UNIFORM),
     n_states_(n_states),
     n_actions_(n_actions),
     val_(val),
     state_actions_prob_()
{
    init_();
}

std::vector<std::pair<uint_t, real_t>>
UniformDiscretePolicy::operator[](uint_t sidx)const {

    return state_actions_prob_[sidx];
}

bool
UniformDiscretePolicy::equals(const DiscretePolicyBase& other)const{

    if(other.type() != PolicyType::DISCRETE_UNIFORM){
        return false;
    }

    if(shape() != dynamic_cast<const UniformDiscretePolicy&>(other).shape()){
        return false;
    }

    if(val_ != dynamic_cast<const UniformDiscretePolicy&>(other).val_){
        return false;
    }

    return true;
}

void
UniformDiscretePolicy::init_(){

    state_actions_prob_.resize(n_states_);

    for(uint_t s=0; s<n_states_; ++s){
        state_actions_prob_[s].resize(n_actions_);

        for(uint_t a=0; a<n_actions_; ++a){
            state_actions_prob_[s][a] = {a, val_};
        }
    }

}

}

}
}
