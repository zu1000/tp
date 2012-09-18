#include "receiver.hpp"

#include <boost/bind.hpp>

#include <comm/service/service.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    receiver::receiver(
            const service::service& service,
            boost::asio::io_service& io,
            const receive_handler& r_handler,
            const error_handler& e_handler)
        : socket_(io)
        , r_handler_(r_handler)
        , e_handler_(e_handler)
    {
        ip::udp::endpoint endpoint(ip::udp::v4(), service.port());
        socket_.open(endpoint.protocol());
        socket_.set_option(ip::udp::socket::reuse_address(true));
        socket_.bind(endpoint);

        // special handling for multicast
        if (service.addr().to_v4().is_multicast())
        {
            socket_.set_option(
                    ip::multicast::join_group(
                        service.addr().to_v4(), service.if_addr().to_v4()));
        }

        // set non_blocking mode
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);

        // assgin the async io handler
        socket_.async_receive_from(
                buffer(r_buffer_, buffer_size),
                remote_endpoint_,
                boost::bind(
                    &receiver::async_receive_handler, this, _1, _2));
    }

    void
    receiver::async_receive_handler(
            const boost::system::error_code& ec,
            std::size_t bytes_transferred)
    {
        r_handler_(
                const_buffer(r_buffer_, bytes_transferred),
                bytes_transferred);

        e_handler_(ec);

        socket_.async_receive_from(
                buffer(r_buffer_, buffer_size),
                remote_endpoint_,
                boost::bind(
                    &receiver::async_receive_handler, this, _1, _2));
    }

}}}}} // tp::comm::io::impl::udp
