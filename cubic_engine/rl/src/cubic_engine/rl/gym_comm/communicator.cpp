#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#endif

#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/gym_comm/requests.h"
#include "zmq/zmq.hpp"


namespace cengine{
namespace rl {
namespace  gym{


Communicator::Communicator(const std::string &url)
    :
      context(),
      socket(),
      url_(url)
{
    context = std::make_unique<zmq::context_t>(1);
    socket = std::make_unique<zmq::socket_t>(*context, ZMQ_PAIR);

    socket->connect(url.c_str());

#ifdef USE_LOG
    kernel::Logger::log_info(get_raw_response());
#endif
}


std::string Communicator::get_raw_response()
{
    // Receive message
    zmq::message_t zmq_msg;
    socket->recv(&zmq_msg);

    // Cast message to string
    std::string response = std::string(static_cast<char *>(zmq_msg.data()), zmq_msg.size());

    return response;
}
}
}
}
