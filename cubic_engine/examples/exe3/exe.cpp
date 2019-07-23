#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace exe
{

using cengine::uint_t;

//forward declaration of the world model
class WorldModel;

// The state estimator of the robot
cengine::ExtendedKalmanFilter ekf;

uint8_t read_world(char* buffer, uint_t bytes ){


}

void handle_connections(uint_t port, boost::asio::io_service& service ){

    boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));


}

}
    
int main(int argc, char** argv) {

    using namespace exe;

    const uint_t PORT = 8001;
    boost::asio::io_service con;

    exe::handle_connections(PORT, con);
    
  return 0;
}

