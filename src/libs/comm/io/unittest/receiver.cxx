#include "receiver.hpp"

#include <cstring>
#include <iostream>

namespace tp {
namespace comm {
namespace io {
namespace unittest {

    receiver::receiver()
        : bytes_transferred_(0)
    {}

    size_t
    receiver::operator ()(
            const const_buffer& buffer,
            std::size_t bytes_transferred)
    {
        bytes_transferred_ = bytes_transferred <= buffer_size
                           ? bytes_transferred
                           : buffer_size;

        const unsigned char* b =
            boost::asio::buffer_cast<const unsigned char*>(buffer);
        std::memcpy(r_buffer_, b, bytes_transferred_);

        std::string msg((char*)b, bytes_transferred);
        std::cout << "recv: " << msg << std::endl;

        return bytes_transferred_;
    }

    bool
    receiver::operator () (
            const error_code& ec)
    {
        ec_ = ec;
        return true;
    }

}}}} // tp::comm::io::unittest
