#ifndef __tp_comm_protocol_internal_statistic_hpp__
#define __tp_comm_protocol_internal_statistic_hpp__

#include "header.hpp"

extern "C" {
#include <stdint.h>
}

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    static const int64_t int64_min = int64_t(1) << 63;

    struct statistic_t
    {
        statistic_t(uint64_t security_id)
            : header_(sizeof(statistic_t), MT_Statistic)
            , security_id_(security_id)
            , high_(int64_min)
            , low_(int64_min)
            , open_(int64_min)
            , close_(int64_min)
            , last_(int64_min)
            , last_quantity_(0)
            , total_volume_(0)
            , utc_timestamp_(0)
        {}

        header_t header_;
        uint64_t security_id_;
        int64_t  high_;
        int64_t  low_;
        int64_t  open_;
        int64_t  close_;
        int64_t settle_;
        int64_t  last_;
        uint64_t last_quantity_;
        uint64_t total_volume_;
        uint64_t utc_timestamp_;
        uint64_t scale_;
    };

}}}} // tp::comm::protocol::internal

#endif//__tp_comm_protocol_internal_statistic_hpp__
