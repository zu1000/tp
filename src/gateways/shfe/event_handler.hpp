#ifndef __tp_gateways_shfe_event_handler_hpp__
#define __tp_gateways_shfe_event_handler_hpp__

#include <comm/protocol/internal/messages.hpp>

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    struct statistic_t;
    struct market_t;

}}}} //tp::comm::protocol::internal

namespace tp {
namespace gateways {
namespace shfe {

    class event_handler
    {
    public:
        virtual void handle(
                const comm::protocol::internal::market_t& market,
                const void* meta_data) = 0;

        virtual void handle(
                const comm::protocol::internal::statistic_t& statistic,
                const void* meta_data) = 0;
    };

}}}// tp::gateways::shfe

#endif//__tp_gateways_shfe_event_handler_hpp__
