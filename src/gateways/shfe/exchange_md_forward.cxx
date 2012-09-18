#include "exchange_md_forward.hpp"

#include <string>

namespace tp {
namespace gateways {
namespace shfe {

    exchange_md_forward::exchange_md_forward(
            comm::io::sender_receiver& sender_receiver)
        : sender_receiver_(sender_receiver)
    {
    }

    void exchange_md_forward::handle(
            const comm::protocol::internal::market_t& market,
            const void* data)
    {
        // ignore at the moment
    }

    void exchange_md_forward::handle(
            const comm::protocol::internal::statistic_t& statistic,
            const void* data)
    {
        // TODO...convert to string
        std::string msg;
        comm::io::const_buffer buffer(msg.c_str(), msg.size());
        comm::io::error_code error;
        sender_receiver_.send(buffer, error);
    }

}}} //tp::gateways::shfe
