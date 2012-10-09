#include "peer.hpp"

#include <boost/bind.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace tcp {

    peer::peer(
            boost::asio::io_service& io)
        : socket_(io)
    {}

    void
    peer::init_peer(
            const receive_handler& r_handler,
            const error_handler& e_handler)
    {
        if (!r_handler || !e_handler)
            throw std::invalid_argument("invalid handler");

        r_handler_ = r_handler;
        e_handler_ = e_handler;

        socket_.async_receive(
                buffer(r_buffer_, buffer_size),
                boost::bind(
                    &peer::async_receive_handler, shared_from_this(), _1, _2));
    }

    std::size_t
    peer::send(
            const const_buffer& buffer,
            error_code& error)
    {
        return socket_.send(boost::asio::buffer(buffer));
    }

    std::string
    peer::peer_address() const
    {
        return socket_.remote_endpoint().address().to_string();
    }

    void
    peer::async_receive_handler(
            const boost::system::error_code& ec,
            std::size_t bytes_transferred)
    {
        r_handler_(
                const_buffer(r_buffer_, bytes_transferred),
                bytes_transferred);

        if (e_handler_(ec) && ec != boost::asio::error::eof)
            socket_.async_receive(
                    buffer(r_buffer_, buffer_size),
                    boost::bind(
                        &peer::async_receive_handler, shared_from_this(), _1, _2));
    }

}}}}} // tp::comm::io::impl::tcp
