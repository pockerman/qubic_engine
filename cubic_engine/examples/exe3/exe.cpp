#include "server.cpp"
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/estimation/extended_kalman_filter.h"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>
#include <array>

namespace exe
{

using cengine::uint_t;

}
    
int main(int argc, char** argv) {

    using namespace exe;

    const uint_t PORT = 8001;
    Server server(PORT, 2);
    server.start();

  return 0;
}

