#ifndef __tp_comm_io_buffer_hpp__
#define __tp_comm_io_buffer_hpp__

#include <boost/asio/buffer.hpp>

namespace tp {
namespace comm {
namespace io {

    static const unsigned buffer_size = 0xffff;

    typedef boost::asio::const_buffer const_buffer;
    typedef boost::asio::mutable_buffer mutable_buffer;

}}} // tp::comm::io

#endif//__tp_comm_io_buffer_hpp__
