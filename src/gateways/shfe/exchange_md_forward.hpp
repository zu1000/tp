#ifndef __tp_gateways_shfe_exchange_md_forward_hpp__
#define __tp_gateways_shfe_exchange_md_forward_hpp__

#include "event_handler.hpp"
#include "client_manager.hpp"
#include "internal_server_manager.hpp"

#include <comm/io/sender_receiver.hpp>
#include <application/options.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    class exchange_md_forward : public event_handler
    {
    public:
        exchange_md_forward(
                boost::asio::io_service& io,
                const std::string& internal_server_config,
                const std::string& local_service);

        void handle(
                const comm::protocol::internal::market_t& market,
                const void* meta_data);
        void handle(
                const comm::protocol::internal::statistic_t& statistic,
                const void* meta_data);

    private:
        boost::scoped_ptr<client_manager> client_manager_;
        boost::scoped_ptr<internal_server_manager> internal_server_manager_;
    };

}}}

#endif//__tp_gateways_shfe_exchange_md_forward_hpp__
