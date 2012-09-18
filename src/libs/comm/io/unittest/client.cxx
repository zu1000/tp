#include "client.hpp"

#include <comm/io/peer.hpp>

#include <boost/asio.hpp>
#include <iostream>

namespace tp {
namespace comm {
namespace io {
namespace unittest {

    client::client(boost::shared_ptr<peer> peer_p)
        : peer_(peer_p)
        , bytes_transferred_(0)
    {}

    std::size_t
    client::async_receive_handle(
            const const_buffer& buffer,
            std::size_t bytes_transferred)
    {
        bytes_transferred_ = bytes_transferred <= buffer_size
                           ? bytes_transferred
                           : buffer_size;

        const unsigned char* b =
            boost::asio::buffer_cast<const unsigned char*>(buffer);
        std::memcpy(r_buffer_, b, bytes_transferred_);

        error_code ec;
        peer_->send(buffer, ec);

        std::string msg((char*)b, bytes_transferred);
        std::cout << "client: " << msg << std::endl;

        return bytes_transferred_;
    }

    bool
    client::error_handle(const error_code& ec)
    {
        std::cout << ec.message() << std::endl;

        if (ec == boost::asio::error::eof)
            return false;

        return true;
    }

    void
    client::send(const std::string& message)
    {
        error_code ec;
        peer_->send(const_buffer(message.c_str(), message.size()), ec);
    }

}}}} // tp::comm::io::unittest
