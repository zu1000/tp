#include "message_handler.hpp"
#include "header.hpp"
#include "service.hpp"
#include "market.hpp"
#include "statistic.hpp"

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    std::size_t message_handler::handle_message(const header_t* msg)
    {
        switch(msg->type_)
        {
            case MT_AddService:
            case MT_ChgService:
            case MT_DelService:
                if (service_handler_)
                    service_handler_(
                            *reinterpret_cast<const service_t*>(msg));
                return sizeof(service_t);
            case MT_Statistic:
                if (statistic_handler_)
                    statistic_handler_(
                            *reinterpret_cast<const statistic_t*>(msg));
                return sizeof(statistic_t);
            case MT_Market:
                if (market_handler_)
                    market_handler_(
                            *reinterpret_cast<const market_t*>(msg));
                return sizeof(market_t);
            default:
                return 0;
        }
    }

    void message_handler::set_service_handler(const service_handler& handler)
    {
        service_handler_ = handler;
    }

    void message_handler::set_market_handler(const market_handler& handler)
    {
        market_handler_ = handler;
    }

    void message_handler::set_statistic_handler(const statistic_handler& handler)
    {
        statistic_handler_ = handler;
    }

}}}} // tp::comm::protocol::internal
