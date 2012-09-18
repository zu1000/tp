/*
 * the raw message struct for service, please refer to TP::COMM::service for
 * app level represent.
 *
 */

#ifndef __tp_comm_protocol_service_h__
#define __tp_comm_protocol_service_h__

#include "header.hpp"

#include <cstring>

extern "C" {
#include <stdint.h>
}

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    struct service_t
    {
        enum ip_version
        {
            V4,
            V6,
            NONE
        };

        service_t(const message_type& type)
            : header_(sizeof(service_t), type)
            , id_(0)
            , type_(0)
            , version_(NONE)
            , port_(0)
        {
            std::memset(&addr_, 0, sizeof(addr_));
        }

        header_t header_;
        uint32_t id_;
        uint32_t type_;
        char     addr_[16];
        uint32_t version_;
        uint32_t port_;
    };

}}}}//tp::comm::protocol::internal

#endif//__tp_comm_protocol_service_h__
