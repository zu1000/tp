#ifndef __tp_comm_io_impl_tcp_base_hpp__
#define __tp_comm_io_impl_tcp_base_hpp__

#include <comm/io/handler.hpp>
#include <comm/io/error.hpp>
#include <comm/io/buffer.hpp>
#include <comm/io/impl/sender.hpp>
#include <comm/io/impl/receiver.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>


namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace tcp {
namespace base {

    class sender : public impl::sender
    {
    public:
        sender(boost::asio::ip::tcp::socket& socket)
            : socket_(socket)
        {}

        std::size_t send(const const_buffer& buffer, error_code& error)
        {
            return socket_.send(boost::asio::buffer(buffer));
        }
    private:
        boost::asio::ip::tcp::socket& socket_;
    };

    class receiver : public impl::receiver
    {
    public:
        receiver(
                boost::asio::ip::tcp::socket& socket,
                const receive_handler& r_handler,
                const error_handler& e_handler)
            : socket_(socket)
            , r_handler_(r_handler)
            , e_handler_(e_handler)
            , offset_(0u)
        {
        }

    protected:
        void do_receive()
        {
            socket_.async_receive(
                    boost::asio::buffer(r_buffer_+offset_, buffer_size-offset_),
                    boost::bind(
                        &receiver::async_receive_handler, this, _1, _2));
        }

    private:

        void async_receive_handler(
                const boost::system::error_code& ec,
                std::size_t bytes_transferred)
        {
            offset_ += bytes_transferred;
            std::size_t size = r_handler_(
                    const_buffer(r_buffer_, bytes_transferred),
                    bytes_transferred);
            offset_ = (offset_ > size) ? (offset_ - size) : 0u;

            if (offset_ && size)
                memmove(r_buffer_, r_buffer_+size, offset_);

            if (e_handler_(ec) && ec != boost::asio::error::eof)
                do_receive();
        }

    private:
        boost::asio::ip::tcp::socket& socket_;
        receive_handler r_handler_;
        error_handler   e_handler_;
        char            r_buffer_[buffer_size];
        std::size_t     offset_;
    };

}}}}}}

#endif//__tp_comm_io_impl_tcp_base_hpp__
