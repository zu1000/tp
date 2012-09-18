#include "sender_receiver.hpp"

#include <comm/service/service.hpp>

#include <boost/bind.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    sender_receiver::sender_receiver(
            const service::service& service,
            boost::asio::io_service& io,
            const receive_handler& r_handler,
            const error_handler& e_handler)
        : socket_(io)
        , target_(service.addr(), service.port())
        , source_()
        , r_handler_(r_handler)
        , e_handler_(e_handler)
        , connected_(false)
    {
        // open socket
        socket_.open(target_.protocol());

        // setup the non blocking
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);

        if (service.addr().to_v4().is_multicast())
        {
            // in multicast case we should reuse the address
            socket_.set_option(ip::udp::socket::reuse_address(true));

            // setup the out bound nic
            socket_.set_option(
                ip::multicast::outbound_interface(service.if_addr().to_v4()));

            // setup the loopback option
            socket_.set_option(ip::multicast::enable_loopback(true));

            // bind to listen on this port
            ip::udp::endpoint local_endpoint(ip::udp::v4(), service.port());
            socket_.bind(local_endpoint);

            // join the group
             socket_.set_option(
                    ip::multicast::join_group(
                        service.addr().to_v4(), service.if_addr().to_v4()));
        }
        else
        {
            // bind the local endpoint
            socket_.bind(socket_.local_endpoint());
        }

        // assgin the async io handler
        socket_.async_receive_from(
                buffer(r_buffer_, buffer_size),
                source_,
                boost::bind(
                    &sender_receiver::async_receive_handler, this, _1, _2));
    }

    std::size_t
    sender_receiver::send(
            const const_buffer& buffer,
            error_code& error)
    {
        if (!connected_)
            return socket_.send_to(
                    boost::asio::buffer(buffer), target_);
        else
            return socket_.send(boost::asio::buffer(buffer));
    }

    void
    sender_receiver::async_receive_handler(
            const boost::system::error_code& ec,
            std::size_t bytes_transferred)
    {
        if (!target_.address().to_v4().is_multicast())
        {
            if (!connected_)
            {
                socket_.connect(source_);
                connected_ = true;
                target_ = source_;
            }

            // this must be handled before we register our handler again, since
            // otherwise it will overwrite the buffer.
            r_handler_(
                    const_buffer(r_buffer_, bytes_transferred),
                    bytes_transferred);

            e_handler_(ec);

            socket_.async_receive(
                    buffer(r_buffer_, buffer_size),
                    boost::bind(
                        &sender_receiver::async_receive_handler, this, _1, _2));
        }
        else
        {

            r_handler_(
                    const_buffer(r_buffer_, bytes_transferred),
                    bytes_transferred);

            e_handler_(ec);

            socket_.async_receive_from(
                    buffer(r_buffer_, buffer_size),
                    source_,
                    boost::bind(
                        &sender_receiver::async_receive_handler, this, _1, _2));
        }

    }

}}}}} // tp::comm::io::impl::udp
