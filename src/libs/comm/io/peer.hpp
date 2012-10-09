#ifndef __tp_comm_io_peer_hpp__
#define __tp_comm_io_peer_hpp__

#include "buffer.hpp"
#include "handler.hpp"

#include <boost/smart_ptr.hpp>

namespace tp {
namespace comm {
namespace io {

namespace impl{
    class peer;
}

    class peer
    {
    public:
        peer(boost::shared_ptr<impl::peer> p);

        void init_peer(
                const receive_handler& r_handler,
                const error_handler& e_handler);

        std::size_t send(
                const const_buffer& buffer,
                error_code& error);

        void peer_id(std::string& id);

        const std::string& peer_id() const;

        std::string peer_address() const;

    private:
        boost::shared_ptr<impl::peer> peer_;
        std::string id_;
    };

}}} // tp::comm::io

#endif//__tp_comm_io_peer_hpp__
