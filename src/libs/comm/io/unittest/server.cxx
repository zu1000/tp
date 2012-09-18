#include "server.hpp"

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

#include <iostream>

namespace tp {
namespace comm {
namespace io {
namespace unittest {

    server::server()
        : io_()
        , acceptor_()
        , clients_()
        , thread_()
    {
    }

    void
    server::start(
            const service::service& service)
    {
        if (thread_) return;

        acceptor_.reset(new acceptor(
                io_, service,
                boost::bind(&server::handle_peer, this, _1)));

        thread_.reset(
                new boost::thread(boost::bind(&server::run, this)));
    }

    void
    server::stop()
    {
        std::cout << ">>>stop" << std::endl;
        io_.stop();
        thread_->join();
        std::cout << "<<<stop" << std::endl;
    }

    void
    server::run()
    {
        std::cout << ">>>run" << std::endl;
        io_.run();
        std::cout << "<<<run" << std::endl;
    }

    void
    server::handle_peer(
            boost::shared_ptr<peer> peer_p)
    {
        std::cout << ">>>handle_peer" << std::endl;
        client_ptr_type client_p(new client(peer_p));

        peer_p->init_peer(
                boost::bind(&client::async_receive_handle, client_p, _1, _2),
                boost::bind(&client::error_handle, client_p, _1));

        client_p->send("connected!");

        clients_.push_back(client_p);
        std::cout << "<<<handle_peer" << std::endl;
    }

}}}} // tp::comm::io::unittest
