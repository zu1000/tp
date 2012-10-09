#include "peer.hpp"

#include <boost/bind.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    peer::peer(
            boost::asio::io_service& io)
        : socket_(io)
        , last_bytes_transferred_(0)
    {
    }

    void
    peer::init_peer(
            const receive_handler& r_handler,
            const error_handler& e_handler)
    {
        // in this case the remote endpoint should be initialized by acceptor
        // otherwise we should throw...
        socket_.connect(remote_endpoint_);

        // set up non blocking mode
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);

        socket_.async_receive(
                buffer(r_buffer_, buffer_size),
                boost::bind(
                    &peer::async_receive_handler, shared_from_this(), _1, _2));

        if (!r_handler || !e_handler)
            throw std::invalid_argument("invalid handler");

        r_handler_ = r_handler;
        e_handler_ = e_handler;

        r_handler_(
                const_buffer(r_buffer_, last_bytes_transferred_),
                last_bytes_transferred_);
    }

    std::size_t
    peer::send(
            const const_buffer& buffer,
            error_code& ec)
    {
        return socket_.send(boost::asio::buffer(buffer));
    }

    std::string
    peer::peer_address() const
    {
        return "";
    }

    void
    peer::async_receive_handler(
            const boost::system::error_code& ec,
            std::size_t bytes_transferred)
    {
        r_handler_(
                const_buffer(r_buffer_, bytes_transferred),
                bytes_transferred);

        last_bytes_transferred_ = bytes_transferred;

        if (e_handler_(ec) && ec != boost::asio::error::eof)
            socket_.async_receive(
                    buffer(r_buffer_, buffer_size),
                    boost::bind(
                        &peer::async_receive_handler, shared_from_this(), _1, _2));
    }

}}}}} // tp::comm::io::impl::udp
