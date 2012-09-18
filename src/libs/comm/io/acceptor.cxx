#include "acceptor.hpp"

#include "impl/tcp/acceptor.hpp"
#include "impl/udp/acceptor.hpp"

namespace tp {
namespace comm {
namespace io {

    acceptor::acceptor(
            boost::asio::io_service& io,
            const service::service& service,
            const peer_handler& p_handler)
        : notifier_(p_handler)
    {
        switch (service.type())
        {
        case service::service::TCP:
            acceptor_.reset(new impl::tcp::acceptor(io, notifier_, service));
            break;
        case service::service::UDP:
        case service::service::Multicast:
            acceptor_.reset(new impl::udp::acceptor(io, notifier_, service));
            break;
        }
    }

}}} // tp::comm::io
