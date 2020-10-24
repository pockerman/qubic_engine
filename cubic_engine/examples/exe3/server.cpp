#include "server.h"
#include "ekf_data.h"
#include "kernel/parallel/threading/simple_task.h"
#include "kernel/base/types.h"

#include <iostream>
#include <thread>
#include <array>

namespace exe
{

struct Server::HandleConnectionsTask: public kernel::SimpleTaskBase<kernel::Null>
{
public:

    HandleConnectionsTask(uint_t PORT, boost::asio::io_service& con)
        :
    kernel::SimpleTaskBase<kernel::Null>(0),
    port_(PORT),
    con_(con)
    {}

    void get_state(boost::asio::ip::tcp::socket& sock);

protected:

    const uint_t port_;
    boost::asio::io_service& con_;

    void run()final override;
};

void
Server::HandleConnectionsTask::run()
{
    using namespace  boost::asio;
    ip::tcp::acceptor acceptor(con_, ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_));

    while (true)
    {
        std::cout<<"Waiting for connection..."<<std::endl;
        ip::tcp::socket sock(con_);
        acceptor.accept(sock);
        std::cout<<"Accepted connection..."<<std::endl;

        get_state(sock);

        sock.close();
    }

    std::cout<<"Finished server thread..."<<std::endl;
}

void
Server::HandleConnectionsTask::get_state(boost::asio::ip::tcp::socket& sock){

    /*std::array<real_t, STATE_SIZE> state_;

    update_F_mat();
    update_B_mat();
    state_estimator.iterate(u, y);

    for(unsigned int i =0; i < state.size(); ++i){
        state_[i] = state[i];
    }

    auto num_bytes_written = sock.write_some(boost::asio::buffer(state_, sizeof(state_)));
    std::cout<<"Num of bytes written: "<<num_bytes_written<<std::endl;
    std::cout<<"State raw bytes size: "<<sizeof(real_t)*state_.size()<<std::endl;*/
}

Server::Server(uint_t port, uint_t nthreads)
    :
   PORT(port),
   con_(),
   pool_(nthreads, false),
   handle_connections_()
{}

void
Server::start()
{
    // initialize the EKF filter
    initialize_ekf();

    pool_.start();
    std::cout<<"Server is listening at port: "<<PORT<<std::endl;
    handle_connections_ = std::make_unique<HandleConnectionsTask>(PORT, con_);
    pool_.add_task(*handle_connections_);
    spin();
}

void
Server::spin(){

    while(true){
       std::this_thread::yield();
    }
}

void
Server::stop()
{
    pool_.close();
}

}
