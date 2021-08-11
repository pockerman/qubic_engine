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
    EpsilonGreedyPolicy(real_t eps, bool reduce=false,
                        real_t min_eps = 0.01, real_t max_eps=1.0 );

    ///
    ///
    ///
    template<typename QMapTp>
    uint_t operator()(const QMapTp& q_map)const;

private:

    real_t eps_init_;
    real_t eps_;
    real_t min_eps_;
    real_t max_eps_;
    bool reduce_eps_;

};

EpsilonGreedyPolicy::EpsilonGreedyPolicy(real_t eps, bool reduce,
                                         real_t min_eps, real_t max_eps)
    :
      eps_init_(eps),
      eps_(eps),
      min_eps_(min_eps),
      max_eps_(max_eps),
      reduce_eps_(reduce)
{}

template<typename QMapTp>
uint_t
EpsilonGreedyPolicy::operator()(const QMapTp& q_map)const{

}

}
}
}

#endif // EPSILON_GREEDY_POLICY_H
