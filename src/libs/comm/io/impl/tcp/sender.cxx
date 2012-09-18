#include "sender.hpp"

#include <comm/service/service.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace tcp {

    sender::sender(
            const service::service& service,
            boost::asio::io_service& io)
        : base::sender(socket_)
        , socket_(io)
    {
        // create endpoint and open
        ip::tcp::endpoint endpoint(service.addr(), service.port());
        socket_.open(endpoint.protocol());

        // configure the socket
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);

        // try connect
        socket_.connect(endpoint);
    }

}}}}} // tp::comm::io::impl::tcp
