#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "boost/noncopyable.hpp"

namespace cengine {
namespace rl {

/// \brief Basic simulation class to run
/// RL experiments. Initial idea
/// was taken from RLlib: https://github.com/HerveFrezza-Buet/RLlib
template<typename ActionTp, typename StateTp, typename RewardTp>
class Simulator: private boost::noncopyable
{
public:

    typedef ActionTp action_t;
    typedef StateTp state_t;
    typedef RewardTp reward_t;

};

}

}

#endif // SIMULATOR_H
