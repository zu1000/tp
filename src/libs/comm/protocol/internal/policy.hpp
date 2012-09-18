#ifndef __tp_comm_protocol_internal_policy_h__
#define __tp_comm_protocol_internal_policy_h__

#include "header.hpp"
#include "message_handler.hpp"

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    class policy
    {
    public:
        typedef header_t        msg_header_type;
        typedef message_handler msg_handler_type;
        const static std::size_t min_pkt_len = sizeof(header_t);
        static std::size_t check_message(const void* pkt, std::size_t& pkt_len);
    };

}}}} // tp::comm::protocol::internal

#endif//__tp_comm_protocol_internal_policy_h__
