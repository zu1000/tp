#ifndef __tp_comm_io_impl_sender_hpp__
#define __tp_comm_io_impl_sender_hpp__

#include <comm/io/buffer.hpp>
#include <comm/io/error.hpp>

namespace tp {
namespace comm {
namespace io {
namespace impl {

    class sender
    {
    public:
        virtual ~sender() = 0;

        virtual size_t send(const const_buffer& buffer, error_code& error) = 0;
    };

}}}} // tp::comm::io::impl

#endif//__tp_comm_io_impl_sender_hpp__
