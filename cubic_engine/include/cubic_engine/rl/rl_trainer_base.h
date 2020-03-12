#ifndef TRAINER_H
#define TRAINER_H

#include <boost/noncopyable.hpp>
namespace cengine {
namespace rl {


template<typename WorldTp, typename AgentTp>
class RLTrainerBase: private boost::noncopyable
{

};

}

}

#endif // TRAINER_H
