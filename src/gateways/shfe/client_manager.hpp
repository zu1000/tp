#ifndef __tp_gateways_shfe_client_manager_hpp__
#define __tp_gateways_shfe_client_manager_hpp__

#include <comm/io/acceptor.hpp>
#include <comm/io/peer.hpp>
#include <comm/io/handler.hpp>
#include <comm/service/service.hpp>

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    class client_manager
    {
        typedef boost::shared_ptr<comm::io::peer> peer_ptr_type;
        typedef comm::io::const_buffer const_buffer;
        typedef comm::io::error_code error_code;
        typedef boost::unordered_map
            <
                const std::string, peer_ptr_type
            > client_peers_type;
    public:
        client_manager(
                boost::asio::io_service& io,
                comm::service::service& s);

        void handle_new_peer(peer_ptr_type peer);

        size_t handle_peer_event(
                peer_ptr_type peer,
                const const_buffer& buffer,
                std::size_t size);

        bool handle_peer_error(
                peer_ptr_type peer,
                const error_code& error);

        peer_ptr_type get_client(const std::string& security_id);

    private:
        comm::io::acceptor acceptor_;
        client_peers_type client_peers_;
    };

}}} // tp::gateways::shfe

#endif//__tp_gateways_shfe_client_manager_hpp__
