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
namespace tcp {

    service::service local_service(
            1, service::service::TCP, "127.0.0.1", 19999, true);

    BOOST_AUTO_TEST_CASE(io_start_stop)
    {
        server local_server;
        local_server.start(local_service);
        local_server.stop();
    }

    BOOST_AUTO_TEST_CASE(simple_tcp_connect)
    {
        server local_server;
        local_server.start(local_service);

        {
            boost::asio::io_service io;
            sender s(local_service, io);
            io.stop();
        }

        // give some time for the acceptor to execute the handle_accept handler
        usleep(10000);

        local_server.stop();

        BOOST_CHECK_EQUAL(local_server.clients_.size(), 1u);
    }

    BOOST_AUTO_TEST_CASE(multiple_tcp_connect)
    {
        server local_server;
        local_server.start(local_service);

        for (int i = 0; i < 2; ++i)
        {
            boost::asio::io_service io;
            sender s(local_service, io);
            io.stop();
        }

        // give some time for the acceptor to execute the handle_accept handler
        usleep(20000);

        local_server.stop();

        BOOST_CHECK_EQUAL(local_server.clients_.size(), 2u);
    }

    BOOST_AUTO_TEST_CASE(simple_tcp_send)
    {
        server local_server;
        local_server.start(local_service);

        std::string data = "dummy_data";

        boost::asio::io_service io;
        sender s(local_service, io);

        usleep(10000);

        error_code ec;
        std::size_t size =
            s.send(const_buffer(data.c_str(), data.size()), ec);

        BOOST_CHECK_EQUAL(size, data.size());

        //io.stop();

        usleep(20000);

        local_server.stop();

        BOOST_CHECK_EQUAL(local_server.clients_.size(), 1u);

        if (local_server.clients_.size())
        {
            client_ptr_type client_p = local_server.clients_[0];

            BOOST_CHECK_EQUAL(client_p->bytes_transferred_, data.size());

            std::string r_data(
                    client_p->r_buffer_, client_p->bytes_transferred_);
            BOOST_CHECK_EQUAL(r_data, data);
        }
    }

    BOOST_AUTO_TEST_CASE(multiple_tcp_send)
    {
        server local_server;
        local_server.start(local_service);

        std::string data1 = "dummy_data 1";
        std::string data2 = "dummy_data 2";

        boost::asio::io_service io;
        sender s1(local_service, io);
        sender s2(local_service, io);

        usleep(20000);

        error_code ec;
        std::size_t size =
            s1.send(const_buffer(data1.c_str(), data1.size()), ec);

        BOOST_CHECK_EQUAL(size, data1.size());

        size =
            s2.send(const_buffer(data2.c_str(), data2.size()), ec);

        BOOST_CHECK_EQUAL(size, data2.size());

        //io.stop();

        usleep(20000);

        local_server.stop();

        BOOST_CHECK_EQUAL(local_server.clients_.size(), 2u);

        if (local_server.clients_.size() == 2u)
        {
            client_ptr_type client_p = local_server.clients_[0];

            BOOST_CHECK_EQUAL(client_p->bytes_transferred_, data1.size());

            std::string r_data1(
                    client_p->r_buffer_, client_p->bytes_transferred_);
            BOOST_CHECK_EQUAL(r_data1, data1);

            client_p = local_server.clients_[1];

            BOOST_CHECK_EQUAL(client_p->bytes_transferred_, data2.size());

            std::string r_data2(
                    client_p->r_buffer_, client_p->bytes_transferred_);
            BOOST_CHECK_EQUAL(r_data2, data2);
        }
    }

    BOOST_AUTO_TEST_CASE(simple_tcp_receiver)
    {
        server local_server;
        local_server.start(local_service);

        boost::asio::io_service io;
        receiver local_r;
        io::receiver r(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r, _1));

        io.run_one();

        std::string data(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(data, "connected!");

        local_server.stop();
    }

    BOOST_AUTO_TEST_CASE(multiple_tcp_receiver)
    {
        server local_server;
        local_server.start(local_service);

        boost::asio::io_service io;
        receiver local_r1;
        receiver local_r2;
        io::receiver r(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r1, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r1, _1));

        r.register_receive_handler(
                boost::bind<std::size_t>(&receiver::operator(), &local_r2, _1, _2));
        r.register_error_handler(
                boost::bind<bool>(&receiver::operator(), &local_r2, _1));

        io.run_one();

        std::string data1(local_r1.r_buffer_, local_r1.bytes_transferred_);
        std::string data2(local_r2.r_buffer_, local_r2.bytes_transferred_);
        BOOST_CHECK_EQUAL(data1, "connected!");
        BOOST_CHECK_EQUAL(data2, "connected!");

        local_server.stop();

        BOOST_CHECK_EQUAL(local_server.clients_.size(), 1u);
    }

    BOOST_AUTO_TEST_CASE(multiple_tcp_receiver2)
    {
        server local_server;
        local_server.start(local_service);

        boost::asio::io_service io;
        receiver local_r1;
        io::receiver r1(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r1, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r1, _1));

        io.run_one();

        receiver local_r2;

        io::receiver r2(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r2, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r2, _1));

        io.run_one();

        std::string data1(local_r1.r_buffer_, local_r1.bytes_transferred_);
        std::string data2(local_r2.r_buffer_, local_r2.bytes_transferred_);
        BOOST_CHECK_EQUAL(data1, "connected!");
        BOOST_CHECK_EQUAL(data2, "connected!");

        local_server.stop();

        BOOST_CHECK_EQUAL(local_server.clients_.size(), 2u);
    }

    BOOST_AUTO_TEST_CASE(simple_tcp_sender_receiver)
    {
        server local_server;
        local_server.start(local_service);

        boost::asio::io_service io;
        receiver local_r;
        sender_receiver sr(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r, _1));

        io.run_one();
        std::string data(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(data, "connected!");

        error_code ec;
        std::string to_server = "sender receiver example";
        sr.send(const_buffer(to_server.c_str(), to_server.size()), ec);

        io.run_one();

        std::string from_server(local_r.r_buffer_, local_r.bytes_transferred_);
        BOOST_CHECK_EQUAL(from_server, to_server);

        local_server.stop();
    }

    BOOST_AUTO_TEST_CASE(multiple_tcp_sender_receiver)
    {
        server local_server;
        local_server.start(local_service);

        boost::asio::io_service io;
        receiver local_r1;
        receiver local_r2;
        sender_receiver sr1(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r1, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r1, _1));

        io.run_one();
        std::string data(local_r1.r_buffer_, local_r1.bytes_transferred_);
        BOOST_CHECK_EQUAL(data, "connected!");

        sender_receiver sr2(
                local_service, io,
                boost::bind<std::size_t>(&receiver::operator(), &local_r2, _1, _2),
                boost::bind<bool>(&receiver::operator(), &local_r2, _1));

        io.run_one();
        data.assign(local_r2.r_buffer_, local_r2.bytes_transferred_);
        BOOST_CHECK_EQUAL(data, "connected!");

        error_code ec;
        std::string to_server = "sender receiver example";
        sr1.send(const_buffer(to_server.c_str(), to_server.size()), ec);

        io.run_one();

        std::string from_server(local_r1.r_buffer_, local_r1.bytes_transferred_);
        BOOST_CHECK_EQUAL(from_server, to_server);

        to_server = "sender receiver example 2";
        sr2.send(const_buffer(to_server.c_str(), to_server.size()), ec);

        io.run_one();
        from_server.assign(local_r2.r_buffer_, local_r2.bytes_transferred_);
        BOOST_CHECK_EQUAL(from_server, to_server);

        local_server.stop();
    }

}}}}} // tp::comm::io::unittest::tcp

