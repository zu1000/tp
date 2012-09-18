#include "acceptor.hpp"

#include <comm/io/impl/acceptor_notifier.hpp>

#include <comm/service/service.hpp>

#include <boost/bind.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    acceptor::acceptor(
            boost::asio::io_service& io,
            impl::acceptor_notifier& notifier,
            const service::service& service)
        : impl::acceptor(notifier)
        , socket_(io)
    {
        // bind to the local endpoint
        ip::udp::endpoint endpoint(ip::udp::v4(), service.port());
        socket_.open(endpoint.protocol());

        // bind the endpoint
        socket_.bind(endpoint);

        // set up the non_blocking option
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);

        // set reuse of the local endpoint
        ip::udp::socket::reuse_address reuse_address(true);
        socket_.set_option(reuse_address);

        // start accept the connection
        start_accept();
    }

    void
    acceptor::start_accept()
    {
        // assgin the async io handler
        boost::shared_ptr<peer> peer_p(
                new peer(socket_.get_io_service()));

        socket_.async_receive_from(
                buffer(r_buffer_, buffer_size),
                peer_p->remote_endpoint_,
                boost::bind(
                    &acceptor::handle_accept, this, peer_p, _1, _2));
    }

    void
    acceptor::handle_accept(
            boost::shared_ptr<peer> peer_p,
            const boost::system::error_code& error,
            std::size_t bytes_transferred)
    {
        std::memcpy(peer_p->r_buffer_, r_buffer_, bytes_transferred);
        peer_p->last_bytes_transferred_ = bytes_transferred;
        notifier_.notifiy_new_peer(peer_p);
        start_accept();
    }

}}}}} // tp::comm::io::impl::udp
