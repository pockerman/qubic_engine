#ifndef TRAINER_H
#define TRAINER_H

#include <boost/noncopyable.hpp>
namespace cengine {
namespace rl {


template<typename WorldTp, typename AgentTp>
class RLTrainerBase: private boost::noncopyable
{

public:

    typedef WorldTp world_t;
    typedef AgentTp agent_t;

    /// \brief Train the agent

};

}

}

#endif // TRAINER_H
