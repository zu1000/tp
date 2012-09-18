#ifndef __tp_comm_protocol_internal_message_handler_hpp__
#define __tp_comm_protocol_internal_message_handler_hpp__

#include "header.hpp"

#include <boost/function.hpp>

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    struct service_t;
    struct market_t;
    struct statistic_t;

    typedef boost::function<void(const service_t&)> service_handler;
    typedef boost::function<void(const market_t&)> market_handler;
    typedef boost::function<void(const statistic_t&)> statistic_handler;

    class message_handler
    {
    public:
        std::size_t handle_message(const header_t* msg);

        void set_service_handler(const service_handler& handler);
        void set_market_handler(const market_handler& handler);
        void set_statistic_handler(const statistic_handler& handler);

    private:
        service_handler   service_handler_;
        market_handler    market_handler_;
        statistic_handler statistic_handler_;
    };


}}}}// tp::comm::protocol::internal

#endif//__tp_comm_protocol_internal_message_hpp__
