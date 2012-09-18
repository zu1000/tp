#ifndef __tp_comm_io_test_server_hpp__
#define __tp_comm_io_test_server_hpp__

#include "client.hpp"

#include <comm/io/peer.hpp>
#include <comm/io/acceptor.hpp>

#include <boost/thread.hpp>
#include <vector>

namespace tp {
namespace comm {

namespace service {
        class service;
}

namespace io {
namespace unittest {

    struct server
    {
        server();

        void start(
                const service::service& service);

        void run();
        
        void stop();

        void handle_peer(
                boost::shared_ptr<peer> peer_p);

        boost::asio::io_service          io_;
        boost::scoped_ptr<acceptor>      acceptor_;
        std::vector<client_ptr_type>     clients_;
        boost::scoped_ptr<boost::thread> thread_;
    };

}}}} // tp::comm::io::unittest

#endif//__tp_comm_io_test_server_hpp__
