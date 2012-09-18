#include "client_manager.hpp"

#include <boost/bind.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    client_manager::client_manager(
            boost::asio::io_service& io,
            comm::service::service& s)
        : acceptor_(io, s,
                boost::bind(&client_manager::handle_new_peer, this, _1))
    {}

    void client_manager::handle_new_peer(client_manager::peer_ptr_type peer)
    {
        peer->init_peer(
                boost::bind(
                    &client_manager::handle_peer_event, this, peer, _1, _2),
                boost::bind(
                    &client_manager::handle_peer_error, this, peer, _1));
    }

    size_t client_manager::handle_peer_event(
            client_manager::peer_ptr_type peer,
            const const_buffer& buffer,
            std::size_t size)
    {
    }

    bool client_manager::handle_peer_error(
            client_manager::peer_ptr_type peer,
            const client_manager::error_code& error)
    {
    }

    std::vector<client_manager::peer_ptr_type>&
    client_manager::get_client(const std::string& security_id)
    {
        static std::vector<peer_ptr_type> empty_vector;

        client_peers_type::iterator find = client_peers_.find(security_id);

        if (find != client_peers_.end())
            return find->second;

        return empty_vector;
    }

}}} // tp::gateways::shfe
