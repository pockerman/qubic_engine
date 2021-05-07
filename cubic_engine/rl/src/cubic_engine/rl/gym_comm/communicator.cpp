#include <memory>
#include <string>

//#include <spdlog/spdlog.h>

#include "cubic_engine/rl/gym_comm/communicator.h"
#include "cubic_engine/rl/gym_comm/requests.h"
#include "third_party/zmq/zmq.hpp"

#include <string>

namespace cengine{
namespace rl {
namespace  gym{



Communicator::Communicator(const std::string &url)
{
    context = std::make_unique<zmq::context_t>(1);
    socket = std::make_unique<zmq::socket_t>(*context, ZMQ_PAIR);

    socket->connect(url.c_str());
    //spdlog::info(get_raw_response());
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
