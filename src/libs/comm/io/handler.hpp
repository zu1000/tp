#ifndef __tp_comm_io_handler_hpp__
#define __tp_comm_io_handler_hpp__

#include "buffer.hpp"
#include "error.hpp"

#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

namespace tp {
namespace comm {
namespace io {

    class peer;

    typedef boost::function<std::size_t(const const_buffer&, std::size_t)> receive_handler;
    typedef boost::function<bool(const error_code&)> error_handler;
    typedef boost::function<bool(const error_code&)> timer_handler;
    typedef boost::function<void(boost::shared_ptr<peer>)> peer_handler;

}}} // tp::comm::io

#endif//__tp_comm_io_handler_hpp__
