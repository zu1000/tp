#ifndef __tp_gateways_shfe_exchange_md_forward_hpp__
#define __tp_gateways_shfe_exchange_md_forward_hpp__

#include "event_handler.hpp"

#include <comm/io/sender_receiver.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    class exchange_md_forward : public event_handler
    {
    public:
        exchange_md_forward(comm::io::sender_receiver& sender_receiver);

        void handle(
                const comm::protocol::internal::market_t& market,
                const void* meta_data);
        void handle(
                const comm::protocol::internal::statistic_t& statistic,
                const void* meta_data);

    private:
        comm::io::sender_receiver& sender_receiver_;
    };

}}}

#endif//__tp_gateways_shfe_exchange_md_forward_hpp__
