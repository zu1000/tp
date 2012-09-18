#include "server.hpp"
#include "receiver.hpp"

#include <comm/io/acceptor.hpp>
#include <comm/io/sender.hpp>
#include <comm/io/receiver.hpp>
#include <comm/io/sender_receiver.hpp>
#include <comm/service/service.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace comm {
namespace io {
namespace unittest {
namespace udp {

    service::service local_service(
            1, service::service::UDP, "127.0.0.1", 19999, true);

    BOOST_AUTO_TEST_CASE(io_start_stop)
    {
        server local_server;
        local_server.start(local_service);
        local_server.stop();
    }

    BOOST_AUTO_TEST_CASE(simpl_udp_sender_and_udp_receiver)
    {
        boost::asio::io_service io;

        receiver local_r;
        io::receiver r(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r, _1));

        io::sender s(local_service, io);

        std::string data = "instance message";
        error_code ec;
        std::size_t sz = s.send(const_buffer(data.c_str(), data.size()), ec);
        BOOST_CHECK_EQUAL(sz, data.size());

        io.run_one();

        std::string result(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(result, data);
    }

    BOOST_AUTO_TEST_CASE(simpl_multicast_sender_and_multicase_receiver)
    {
        service::service multicast_service(
            1, service::service::Multicast, "239.0.32.4", 44332, true, "127.0.0.1");

        boost::asio::io_service io;

        receiver local_r;
        io::receiver r(
                multicast_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r, _1));

        io::sender s(multicast_service, io);

        std::string data = "instance message";
        error_code ec;
        std::size_t sz = s.send(const_buffer(data.c_str(), data.size()), ec);
        BOOST_CHECK_EQUAL(sz, data.size());

        io.run_one();

        std::string result(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(result, data);
    }

    BOOST_AUTO_TEST_CASE(simpl_udp_sender_receiver_and_udp_server)
    {
        server local_server;
        local_server.start(local_service);

        boost::asio::io_service io;

        receiver local_r;
        io::sender_receiver sr(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r, _1));

        std::string data = "instance message";

        // the first send should establish the "connection" to server
        error_code ec;
        std::size_t sz = sr.send(const_buffer(data.c_str(), data.size()), ec);
        BOOST_CHECK_EQUAL(sz, data.size());

        // try to receive the message back..
        io.run_one();

        std::string result(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(data, result);

        io.run_one();
        result.assign(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(result, "connected!");

        // give some time to get the correct result
        usleep(10000);

        BOOST_CHECK_EQUAL(local_server.clients_.size(), 1u);

        // second send that shoud use socket::send instead of socket::sent_to
        data += data;
        sz = sr.send(const_buffer(data.c_str(), data.size()), ec);
        BOOST_CHECK_EQUAL(sz, data.size());

        io.run_one();
        result.assign(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(data, result);

        local_server.stop();
    }

    BOOST_AUTO_TEST_CASE(simpl_multicast_sender_receiver)
    {
        service::service multicast_service(
            1, service::service::Multicast, "239.0.32.5", 44332, true, "127.0.0.1");

        boost::asio::io_service io;

        receiver local_r;
        io::sender_receiver sr(
                multicast_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r, _1));

        std::string data = "instance message";

        // the first send should establish the "connection" to server
        error_code ec;
        std::size_t sz = sr.send(const_buffer(data.c_str(), data.size()), ec);
        BOOST_CHECK_EQUAL(sz, data.size());

        // try to receive the message back..
        io.run_one();

        std::string result(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(data, result);
    }

}}}}} // tp::comm::io::unittest::udp
