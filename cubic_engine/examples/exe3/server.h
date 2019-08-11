#ifndef SERVER_H
#define SERVER_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "kernel/executors/thread_pool.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
namespace exe
{

using cengine::uint_t;

class Server
{
public:

    /// \brief Constructor
    Server(uint_t port, uint_t nthreads=1);

    /// \brief Destructor
    ~Server(){stop();}

    /// \brief Start the server
    void start();

    /// \brief spin the server main thread
    void spin();

    /// \brief Stop the server
    void stop();

private:

    /// \brief The boost::asio connection that the server is using
    boost::asio::io_service con_;

    /// \brief the port the server accepts requests
    const uint_t PORT;

    /// \brief the thread pool that actually does the work
    kernel::ThreadPool pool_;

    /// \brief The task that handles the connections
    struct HandleConnectionsTask;

    std::unique_ptr<HandleConnectionsTask> handle_connections_;

};


}

#endif // SERVER_H
