#ifndef __tp_comm_io_impl_tcp_sender_receiver_hpp__
#define __tp_comm_io_impl_tcp_sender_receiver_hpp__

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

    class sender_receiver : public base::sender,
                            public base::receiver
    {
    public:
        sender_receiver(
                const service::service& service,
                boost::asio::io_service& io,
                const receive_handler& r_handler,
                const error_handler& e_handler);

    private:
        boost::asio::ip::tcp::socket socket_;
    };

}}}}} // tp::comm::io::impl::tcp
#endif//__tp_comm_io_impl_tcp_sender_receiver_hpp__
