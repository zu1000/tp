#ifndef __tp_comm_io_impl_tcp_sender_hpp__
#define __tp_comm_io_impl_tcp_sender_hpp__

#include "base.hpp"

namespace tp {
namespace comm {
namespace service {

    class service;

}}} // tp::comm::service

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace tcp {

    class sender : public base::sender
    {
    public:
        sender(
                const service::service& service,
                boost::asio::io_service& io);

    private:
        boost::asio::ip::tcp::socket socket_;
    };

}}}}} // tp::comm::io::impl::tcp

#endif//__tp_comm_io_impl_tcp_sender_hpp__
