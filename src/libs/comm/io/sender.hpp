#ifndef __tp_comm_io_sender_hpp__
#define __tp_comm_io_sender_hpp__

#include "buffer.hpp"
#include "error.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/shared_ptr.hpp>

namespace tp {
namespace comm {
namespace service {

    class service;

}}}//tp::comm::service

namespace tp {
namespace comm {
namespace io {

namespace impl {
    class sender;
}

    class sender
    {
    public:
        sender(
            const service::service& service,
            boost::asio::io_service& io);

        std::size_t send(const const_buffer& buffer, error_code& error);

    private:
        boost::shared_ptr<impl::sender> sender_;
    };

}}} // tp::comm::io

#endif//__tp_comm_io_sender_hpp__
