#ifndef __tp_comm_io_impl_peer_hpp__
#define __tp_comm_io_impl_peer_hpp__

#include <comm/io/buffer.hpp>
#include <comm/io/handler.hpp>

namespace tp {
namespace comm {
namespace io {
namespace impl {

    class peer
    {
    public:
        virtual ~peer() = 0;

        virtual void init_peer(
                const receive_handler& r_handler,
                const error_handler& e_handler) = 0;

        virtual std::size_t send(
                const const_buffer& buffer,
                error_code& error) = 0;
    };

}}}} // tp::comm:io::impl

#endif//__tp_comm_io_impl_peer_hpp__
