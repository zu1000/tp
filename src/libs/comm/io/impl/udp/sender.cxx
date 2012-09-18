#include "sender.hpp"

#include <comm/service/service.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    sender::sender(
            const service::service& service,
            boost::asio::io_service& io)
        : socket_(io)
        , target_(service.addr(), service.port())
    {
        // create endpoint and try connect
        socket_.open(target_.protocol());

        // setup non_blocking
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);

        // enable loop_back option
        if (service.addr().to_v4().is_multicast());
        {
            socket_.set_option(
                ip::multicast::outbound_interface(service.if_addr().to_v4()));

            socket_.set_option(ip::multicast::enable_loopback(true));
        }

        // set reuse of the local port
        ip::udp::socket::reuse_address reuse_address(true);
        socket_.set_option(reuse_address);

        // bind to the local endpoint
        socket_.bind(socket_.local_endpoint());
    }

    std::size_t
    sender::send(const const_buffer& buffer, error_code& error)
    {
        return socket_.send_to(boost::asio::buffer(buffer), target_);
    }

}}}}} // tp::comm::io::impl::udp
