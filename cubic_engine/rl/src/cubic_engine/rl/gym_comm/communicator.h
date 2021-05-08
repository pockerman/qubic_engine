#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "cubic_engine/base/cubic_engine_types.h"

#include <msgpack.hpp>
//#include <spdlog/spdlog.h>
//#include <spdlog/fmt/bundled/ostream.h>

#include "requests.h"
#include "zmq/zmq.hpp"

#include <memory>
#include <string>
#include <sstream>
#include <iostream>

namespace cengine{
namespace rl {
namespace gym {

///
/// \brief The Communicator class
///
class Communicator
{
  public:

    ///
    /// \brief Communicator
    /// \param url
    ///
    Communicator(const std::string &url);

    ///
    ///
    ///
    ~Communicator()=default;

    ///
    /// \brief get_raw_response
    /// \return
    ///
    std::string get_raw_response();

    ///
    ///
    ///
    template <class T>
    std::unique_ptr<T> get_response();

    ///
    /// \brief Send the request
    ///
    template <class T>
    void send_request(const Request<T> &request);

  private:


    std::unique_ptr<zmq::context_t> context;
    std::unique_ptr<zmq::socket_t> socket;

    ///
    /// \brief url_ The URL to use
    ///
    std::string url_;
};

template <class T>
std::unique_ptr<T>
Communicator::get_response()
{
    // Receive message
    zmq::message_t packed_msg;
    socket->recv(&packed_msg);

    // Desrialize message
    msgpack::object_handle object_handle = msgpack::unpack(static_cast<char *>(packed_msg.data()), packed_msg.size());
    msgpack::object object = object_handle.get();

    // Fill out response object
    std::unique_ptr<T> response = std::make_unique<T>();
    try
    {
        object.convert(response);
    }
    catch (...)
    {
        //spdlog::error("Communication error: {}", object);
    }

    return response;
}

template <class T>
void
Communicator::send_request(const Request<T> &request)
{
    msgpack::sbuffer buffer;
    msgpack::pack(buffer, request);

    zmq::message_t message(buffer.size());
    std::memcpy(message.data(), buffer.data(), buffer.size());
    socket->send(message);
}


}
}

}

#endif
