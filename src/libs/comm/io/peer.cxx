#include "peer.hpp"
#include "impl/peer.hpp"

namespace tp {
namespace comm {
namespace io {

    peer::peer(boost::shared_ptr<impl::peer> p)
        : peer_(p)
    {}

    void
    peer::init_peer(
            const receive_handler& r_handler,
            const error_handler& e_handler)
    {
        peer_->init_peer(r_handler, e_handler);
    }

    std::size_t
    peer::send(
            const const_buffer& buffer,
            error_code& error)
    {
        return peer_->send(buffer, error);
    }

    void
    peer::peer_id(std::string& id)
    {
        id_ = id;
    }

    const std::string&
    peer::peer_id() const
    {
        return id_;
    }

    std::string
    peer::peer_address() const
    {
        return peer_->peer_address();
    }

}}} // tp::comm::io
