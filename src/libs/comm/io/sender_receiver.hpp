#ifndef __tp_comm_io_sender_receiver_hpp__
#define __tp_comm_io_sender_receiver_hpp__

#include "handler.hpp"
#include "error.hpp"
#include "impl/sender_receiver.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>

namespace tp {
namespace comm {
namespace service {

    class service;

}}} // tp::comm::service

namespace tp {
namespace comm {
namespace io {

    class sender_receiver
    {
    public:
        sender_receiver(
                const service::service& service,
                boost::asio::io_service& io,
                const receive_handler& r_handler,
                const error_handler& e_handler);

        std::size_t send(const const_buffer& buffer, error_code& error);

    private:
        boost::shared_ptr<impl::sender_receiver> sender_receiver_;
    };

}}} // tp::comm::io

#endif//__tp_comm_io_sender_receiver_hpp__
