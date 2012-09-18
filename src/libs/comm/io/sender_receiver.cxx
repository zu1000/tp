#include "sender_receiver.hpp"

#include "impl/tcp/sender_receiver.hpp"
#include "impl/udp/sender_receiver.hpp"

#include <comm/service/service.hpp>

namespace tp {
namespace comm {
namespace io {

    sender_receiver::sender_receiver(
            const service::service& service,
            boost::asio::io_service& io,
            const receive_handler& r_handler,
            const error_handler& e_handler)
    {
        switch (service.type())
        {
        case service::service::TCP:
            sender_receiver_.reset(
                    new impl::tcp::sender_receiver(
                        service, io, r_handler, e_handler));
            break;
        case service::service::UDP:
        case service::service::Multicast:
            sender_receiver_.reset(
                    new impl::udp::sender_receiver(
                        service, io, r_handler, e_handler));
            break;
        }
    }

    std::size_t
    sender_receiver::send(
            const const_buffer& buffer,
            error_code& error)
    {
        return sender_receiver_->send(buffer, error);
    }

}}} // tp::comm::io
