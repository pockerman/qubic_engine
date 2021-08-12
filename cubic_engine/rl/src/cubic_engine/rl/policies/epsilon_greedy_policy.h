#ifndef EPSILON_GREEDY_POLICY_H
#define EPSILON_GREEDY_POLICY_H

#include "cubic_engine/base/cubic_engine_types.h"

namespace cengine {
namespace rl {
namespace policies {

///
/// \brief The EpsilonGreedyPolicy class
///
class EpsilonGreedyPolicy
{
public:

    ///
    /// \brief EpsilonGreedyPolicy
    /// \param eps
    /// \param reduce
    /// \param min_eps
    /// \param max_eps
    ///
    explicit EpsilonGreedyPolicy(real_t eps, uint_t n_actions, uint_t seed=0,
                                 bool reduce=false, real_t min_eps = 0.01, real_t max_eps=1.0 );

    ///
    ///
    ///
    template<typename QMapTp>
    uint_t operator()(const QMapTp& q_map, uint_t state)const;

    ///
    /// \brief adjust_on_episode
    /// \param episode
    ///
    void adjust_on_episode(uint_t episode);

private:

    real_t eps_init_;
    real_t eps_;
    real_t min_eps_;
    real_t max_eps_;
    uint_t n_actions_;
    uint_t seed_;
    bool reduce_eps_;

    mutable std::uniform_real_distribution<> real_dist_;
    mutable std::uniform_int_distribution<> distrib_;
};

EpsilonGreedyPolicy::EpsilonGreedyPolicy(real_t eps,  uint_t n_actions, uint_t seed,
                                         bool reduce, real_t min_eps, real_t max_eps)
    :
      eps_init_(eps),
      eps_(eps),
      min_eps_(min_eps),
      max_eps_(max_eps),
      n_actions_(n_actions_),
      seed_(seed),
      reduce_eps_(reduce),
      real_dist_(0.0, 1.0),
      distrib_(0,  n_actions_ - 1)
{}

template<typename QMapTp>
uint_t
EpsilonGreedyPolicy::operator()(const QMapTp& q_map, uint_t state)const{


    std::mt19937 gen(seed_);


    if(real_dist_(gen) > eps_){
        // select greedy action with probability epsilon
        return blaze::max(q_map[state]);
    }

    return distrib_(gen);
}

void
EpsilonGreedyPolicy::adjust_on_episode(uint_t episode){
    if(reduce_eps_){
        eps_ /= eps_ / episode;
    }
}

}
}
}

#endif // EPSILON_GREEDY_POLICY_H
