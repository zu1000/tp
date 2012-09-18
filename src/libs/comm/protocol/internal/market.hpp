#ifndef __tp_comm_protocol_internal_market_hpp__
#define __tp_comm_protocol_internal_market_hpp__

#include "header.hpp"

extern "C" {
#include <stdint.h>
}

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    // no construct and we don't want to init the space in order to reduce
    // overhead of initialization
    struct market_level_t
    {
        enum market_level_flag
        {
            bid     = 1 << 0,
            implied = 1 << 1
        };

        bool is_bid()
        {
            return flags_ & bid;
        }

        bool is_implied()
        {
            return flags_ & implied;
        }

        int64_t  price_;    // price
        uint64_t quantity_; // quantity
        uint64_t flags_;    // flags...
        uint64_t id_;       // possible level id for non-aggregated
    };

    struct market_t
    {
        static const size_t array_size = 100;
        market_t(uint64_t security_id)
            : header_(
                sizeof(market_t) - array_size*sizeof(market_level_t), MT_Market)
            , security_id_(security_id)
            , flags_(0)
            , sequence_(0)
            , utc_time_stamp_(0)
            , number_of_levels_(0)
        {}

        enum market_flag
        {
            // last: indicate if this is the last part of the update
            last    = 1 << 0
        };

        bool is_last()
        {
            return flags_ & last;
        }

        header_t header_;
        uint64_t security_id_;
        uint64_t flags_;
        uint64_t sequence_;
        uint64_t utc_time_stamp_;
        uint32_t number_of_levels_;
        market_level_t levels[100];
    };

}}}}

#endif//__tp_comm_protocol_market_hpp__
