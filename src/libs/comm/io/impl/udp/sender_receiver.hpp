#ifndef __tp_comm_io_impl_udp_sender_receiver_hpp__
#define __tp_comm_io_impl_udp_sender_receiver_hpp__

#include <comm/io/impl/sender_receiver.hpp>

#include <boost/asio.hpp>

namespace tp {
namespace comm {
namespace service {

    class service;

}}} // tp::comm::service

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    class sender_receiver : public impl::sender_receiver
    {
    public:
        sender_receiver(
                const service::service& service,
                boost::asio::io_service& io,
                const receive_handler& r_handler,
                const error_handler& e_handler);

        std::size_t send(
                const const_buffer& buffer,
                error_code& error);

    private:
        void async_receive_handler(
                const boost::system::error_code& ec,
                std::size_t bytes_transferred);

    private:
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint target_;
        boost::asio::ip::udp::endpoint source_;
        receive_handler r_handler_;
        error_handler   e_handler_;
        char            r_buffer_[buffer_size];
        bool            connected_;
    };

}}}}} // tp::comm::io::impl::udp
#endif//__tp_comm_io_impl_udp_sender_receiver_hpp__
