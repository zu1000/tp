#include "client_manager.hpp"
#include "utils.hpp"

#include <application/options.hpp>

#include <boost/bind.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    static bool parse_request(
            const std::string& req, std::string& security, std::string& user_id)
    {
        size_t pos = req.find(",");

        if (pos == std::string::npos)
            return false;

        security = req.substr(0, pos);
        user_id = req.substr(pos+1);

        return true;
    }

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

        std::cout << "Accepted client from: " << peer->peer_address() << std::endl;
    }

    size_t client_manager::handle_peer_event(
            client_manager::peer_ptr_type peer,
            const const_buffer& buffer,
            std::size_t size)
    {
        std::string req(boost::asio::buffer_cast<const char*>(buffer), size);

        if (!size) return size;

        std::cout << "Received subscription for contract: " << req << std::endl;

        std::string security, user_id;

        size_t pos = req.find("\n");
        size_t handled_size = 0;

        while (pos != std::string::npos)
        {
            if (parse_request(req.substr(0, pos), security, user_id))
                client_peers_[security].push_back(peer);

            req.erase(0, pos+1);
            handled_size += pos+1;

            pos = req.find("\n");
        }

        return handled_size;
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

            try
            {
                if (clients[i])
                    clients[i]->send(
                            comm::io::const_buffer(msg.c_str(), msg.size()), error);
            }
            catch(...)
            {
                // assume that cnnection is gone... delete the client
                clients[i].reset();
            }
        }
    }

}}} // tp::gateways::shfe
