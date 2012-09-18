#include "sender_receiver.hpp"

#include <comm/service/service.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace tcp {

    sender_receiver::sender_receiver(
            const service::service& service,
            boost::asio::io_service& io,
            const receive_handler& r_handler,
            const error_handler& e_handler)
        : base::sender(socket_)
        , base::receiver(socket_, r_handler, e_handler)
        , socket_(io)
    {
        // create endpoint and open socket
        ip::tcp::endpoint endpoint(service.addr(), service.port());
        socket_.open(endpoint.protocol());

        // configurae the socket
        socket_base::non_blocking_io non_blocking(true);
        socket_.io_control(non_blocking);
        //try connect
        socket_.connect(endpoint);

        // assgin the async io handler
        do_receive();
    }

}}}}} //tp::comm::io::impl::tcp
