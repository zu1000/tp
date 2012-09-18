#ifndef __tp_comm_io_receiver_hpp__
#define __tp_comm_io_receiver_hpp__

#include "handler.hpp"
#include "error.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/smart_ptr.hpp>

#include <vector>

namespace tp {
namespace comm {
namespace service {

    class service;

}}} // tp::comm::service

namespace tp {
namespace comm {
namespace io {

namespace impl{
    class receiver;
}

    class receiver
    {
    public:
        receiver(
                const service::service& service,
                boost::asio::io_service& io,
                const receive_handler& r_handler,
                const error_handler& e_handler);

        std::size_t register_receive_handler(
                const receive_handler& r_handler);

        std::size_t register_error_handler(
                const error_handler& e_handler);

    private:
        std::size_t async_receive_handler(
                const const_buffer& buffer,
                std::size_t bytes_transferred);

        bool async_error_handler(
                const error_code& ec);

    private:
        boost::shared_ptr<impl::receiver> receiver_;
        std::vector<receive_handler>      r_handlers_;
        std::vector<error_handler>        e_handlers_;
    };

    typedef boost::scoped_ptr<receiver>           receiver_ptr;
}}}


#endif//__tp_comm_io_receiver_hpp__
