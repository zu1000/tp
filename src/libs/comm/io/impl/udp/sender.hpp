#ifndef __tp_comm_io_udp_impl_sender_hpp__
#define __tp_comm_io_udp_impl_sender_hpp__

#include <comm/io/impl/sender.hpp>

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

    class sender : public impl::sender
    {
    public:
        sender(
                const service::service& service,
                boost::asio::io_service& io);

        std::size_t send(
                const const_buffer& buffer,
                error_code& error);

    private:
        boost::asio::ip::udp::socket   socket_;
        boost::asio::ip::udp::endpoint target_;
    };

}}}}} // tp::comm::io::impl::udp

#endif//__tp_comm_io_impl_udp_sender_hpp__
