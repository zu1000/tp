#ifndef __tp_comm_io_test_client_hpp__
#define __tp_comm_io_test_client_hpp__

#include <comm/io/buffer.hpp>
#include <comm/io/error.hpp>
#include <boost/smart_ptr.hpp>

namespace tp {
namespace comm {
namespace io {

    class peer;

namespace unittest {

    struct client
    {
        client(boost::shared_ptr<peer> peer_p);

        std::size_t async_receive_handle(
                const const_buffer& buffer,
                std::size_t bytes_transferred);

        bool error_handle(const error_code& ec);

        void send(const std::string& message);

        boost::shared_ptr<peer> peer_;
        char                    r_buffer_[buffer_size];
        std::size_t             bytes_transferred_;
    };

    typedef boost::shared_ptr<client> client_ptr_type;

}}}} // tp::comm::io::unittest
#endif//__tp_comm_io_test_client_hpp__
