#include "client_manager.hpp"
#include "utils.hpp"

#include <application/options.hpp>

#include <boost/bind.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    client_manager::client_manager(
            boost::asio::io_service& io,
            const std::string& config_file)
    {
        application::options options("client_manager");

        const char* local_service_port = "local_service_port";
        const char* local_nic_ip = "local_nic_ip";

        add_option<unsigned short>(options, local_service_port);
        add_option<std::string>(options, local_nic_ip);

        if (!options.parse(config_file))
            throw std::invalid_argument("Invalid local service config");

        unsigned short port;
        std::string nic;

        get_mandatory_option(
                options, "local_service_port", port);
        get_optional_option(
                options, "local_nic_ip", nic);

        comm::service::service s(
                -1, comm::service::service::TCP,
                "127.0.0.1", port, true, nic);

        acceptor_.reset(
                new comm::io::acceptor(
                    io, s,
                    boost::bind(&client_manager::handle_new_peer, this, _1)));
    }

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

    void
    client_manager::send(const std::string& security_id, const std::string& msg)
    {
        client_peers_type::iterator find = client_peers_.find(security_id);

        if (find == client_peers_.end())
            return;

        std::vector<peer_ptr_type>& clients = find->second;

        for (size_t i = 0; i < clients.size(); ++i)
        {
            comm::io::error_code error;
            clients[i]->send(
                    comm::io::const_buffer(msg.c_str(), msg.size()), error);
        }
    }

}}} // tp::gateways::shfe
