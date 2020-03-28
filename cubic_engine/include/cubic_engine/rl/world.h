#ifndef WORLD_H
#define WORLD_H

#include "boost/noncopyable.hpp"

namespace cengine {
namespace rl {

/// \brief
template<typename ActionTp, typename StateTp, typename RewardTp>
class World: private boost::noncopyable
{
public:

    typedef ActionTp action_t;
    typedef StateTp state_t;
    typedef RewardTp reward_t;

    /// \brief Destructor
    ~World(){}

    /// \brief Returns the state
    virtual state_t& sense()=0;

    /// \brief Transition to a new state by
    /// performing the given action
    virtual void step(const action_t&)=0;

    /// \brief Returns the reward associated
    /// with the last state transition
    virtual reward_t reward()const=0;

protected:

    /// \brief Constructor protected
    World()=default;

};

}

}

#endif // WORLD_H
