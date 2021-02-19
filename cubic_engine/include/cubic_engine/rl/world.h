#ifndef WORLD_H
#define WORLD_H

#include "cubic_engine/base/config.h"

#ifdef USE_RL

#include "cubic_engine/base/config.h"
#include "boost/noncopyable.hpp"

namespace cengine {
namespace rl {

///
/// \brief The World class. Base class for
/// modeling environments in reinforcement learning
/// algorithms
///
template<typename ActionTp, typename StateTp, typename RewardTp>
class World: private boost::noncopyable
{
public:

    typedef ActionTp action_t;
    typedef StateTp state_t;
    typedef RewardTp reward_t;
    typedef typename RewardTp::value_t reward_value_t;

    ///
    /// \brief Destructor
    ///
    ~World(){}

    ///
    /// \brief Returns the state
    ///
    virtual const state_t& sense()=0;


protected:

    ///
    /// \brief Constructor protected
    ///
    World()=default;

    ///
    /// \brief reward_. The object that handles
    /// reward generaion
    ///
    reward_t reward_;

};

}
}

#endif
#endif // WORLD_H
