#include "receiver.hpp"

#include <comm/service/service.hpp>

#include <boost/bind.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace tcp {

    receiver::receiver(
            const service::service& service,
            boost::asio::io_service& io,
            const receive_handler& r_handler,
            const error_handler& e_handler)
        : base::receiver(socket_, r_handler, e_handler)
        , socket_(io)
    {
        if (!r_handler || !e_handler)
            throw std::invalid_argument("invalid handler");

        // create endpoint and open
        ip::tcp::endpoint endpoint(service.addr(), service.port());
        socket_.open(endpoint.protocol());

        // configure the socket
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);

        // try connect
        socket_.connect(endpoint);

        // start receiving
        do_receive();
    }

}}}}} // tp::comm::io::impl::tcp
