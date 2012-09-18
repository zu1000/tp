/*
 * packet header definition
 *
 */

#ifndef __tp_comm_protocol_header_h__
#define __tp_comm_protocol_header_h__

#include "message_type.hpp"

#include <boost/asio/buffer.hpp>

#include <stdexcept>

extern "C" {
#include <stdint.h>
}

#ifndef INTERNAL_PROTOCOL_VERSION
#define INTERNAL_PROTOCOL_VERSION 2012012900
#endif

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    struct header_t
    {
        header_t(uint32_t size, uint32_t type)
            : version_(INTERNAL_PROTOCOL_VERSION)
            , size_(size)
            , type_(type)
        {}

        uint32_t version_;
        uint32_t size_;
        uint32_t type_;
    };

    // check the validity of message.
    //   - not valid -> return MT_Unknown
    //   - valid -> return message type
    // throw std::runtime_error:
    //   in case the message type is invalid
    static uint32_t check_message(const boost::asio::const_buffer& buffer)
    {
        std::size_t size = boost::asio::buffer_size(buffer);

        if (size < sizeof(header_t))
            return MT_Unknown;

        const header_t& header =
            *boost::asio::buffer_cast<const header_t*>(buffer);

        if (size < header.size_)
            return MT_Unknown;

        if (header.type_ >= MT_Unknown)
            throw std::runtime_error("invalid message!");

        return header.type_;
    }

}}}}//tp::comm::protocol::internal

#endif//__tp_comm_protocol_header_h__
