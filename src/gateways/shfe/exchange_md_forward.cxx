#include "exchange_md_forward.hpp"

#include <string>

namespace tp {
namespace gateways {
namespace shfe {

    exchange_md_forward::exchange_md_forward(
            boost::asio::io_service& io,
            const std::string& internal_server_config,
            const std::string& local_service)
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
        //sender_receiver_.send(buffer, error);
    }

}}} //tp::gateways::shfe
