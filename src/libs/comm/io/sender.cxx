#include "sender.hpp"

#include "impl/tcp/sender.hpp"
#include "impl/udp/sender.hpp"

#include <comm/service/service.hpp>

namespace tp {
namespace comm {
namespace io {

    sender::sender(
            const service::service& service,
            boost::asio::io_service& io)
    {
        switch (service.type())
        {
        case service::service::TCP:
            sender_.reset(new impl::tcp::sender(service, io));
            break;
        case service::service::UDP:
        case service::service::Multicast:
            sender_.reset(new impl::udp::sender(service, io));
            break;
        }
    }

    std::size_t
    sender::send(
            const const_buffer& buffer,
            error_code& error)
    {
        return sender_->send(buffer, error);
    }

}}} // tp::comm::io
