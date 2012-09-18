#include "acceptor.hpp"

#include <comm/io/impl/acceptor_notifier.hpp>

#include <comm/service/service.hpp>

#include <boost/bind.hpp>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace tcp {

    acceptor::acceptor(
            boost::asio::io_service& io,
            impl::acceptor_notifier& notifier,
            const service::service& service)
        : impl::acceptor(notifier)
        , acceptor_(io)
    {
        ip::tcp::endpoint endpoint(ip::tcp::v4(), service.port());
        acceptor_.open(endpoint.protocol());

        ip::tcp::acceptor::reuse_address reuse_address(true);
        acceptor_.set_option(reuse_address);

        acceptor_.bind(endpoint);
        acceptor_.listen();

        start_accept();
    }

    void
    acceptor::start_accept()
    {
        boost::shared_ptr<peer> peer_p(
            new peer(acceptor_.get_io_service()));

        acceptor_.async_accept(
                peer_p->socket_,
                boost::bind(&acceptor::handle_accept, this, peer_p, _1));
    }

    void
    acceptor::handle_accept(
            boost::shared_ptr<peer> peer_p,
            const boost::system::error_code& error)
    {
        notifier_.notifiy_new_peer(peer_p);
        start_accept();
    }

}}}}} // tp::comm::io::impl::tcp
