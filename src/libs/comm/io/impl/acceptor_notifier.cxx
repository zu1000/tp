#include "acceptor_notifier.hpp"
#include "peer.hpp"

#include <comm/io/acceptor_listener.hpp>
#include <comm/io/peer.hpp>

namespace tp {
namespace comm {
namespace io {
namespace impl {

    acceptor_notifier::acceptor_notifier(
            const peer_handler& p_handler)
        : p_handler_(p_handler)
    {}

    void
    acceptor_notifier::notifiy_new_peer(
            boost::shared_ptr<peer> p_impl)
    {
        boost::shared_ptr<io::peer> p(new io::peer(p_impl));
        p_handler_(p);
    }

}}}}//tp::comm::io::impl
