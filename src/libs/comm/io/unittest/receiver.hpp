#ifndef __tp_comm_io_test_handler_hpp__
#define __tp_comm_io_test_handler_hpp__

#include <comm/io/buffer.hpp>
#include <comm/io/error.hpp>

namespace tp {
namespace comm {
namespace io {
namespace unittest {

    struct receiver
    {
        receiver();

        size_t operator()(
                const const_buffer& buffer,
                std::size_t bytes_transferred);

        bool operator()(
                const error_code& ec);

        char        r_buffer_[buffer_size];
        std::size_t bytes_transferred_;
        error_code  ec_;
    };

}}}} // tp::comm::io::unittest
#endif//__tp_comm_io_test_handler_hpp__
