#include "exchange_md_forward.hpp"

#include "utils.hpp"

#include <boost/lexical_cast.hpp>

#include <string>
#include <sstream>
#include <iomanip>

namespace tp {
namespace gateways {
namespace shfe {

    inline std::string build_price(int64_t price, int64_t scale)
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(4) << (double(price)/scale);
        return ss.str();
    }

    inline void build_message(
            const std::string& security_id,
            const std::string& timestamp,
            const comm::protocol::internal::statistic_t& statistic,
            std::string& msg, bool longmsg)
    {
        double scale = statistic.scale_;

        if (longmsg)
        {
            msg = security_id + "," +
                    timestamp + "," +
                    build_price(statistic.last_, statistic.scale_) + "," +
                    build_price(statistic.open_, statistic.scale_) + "," +
                    build_price(statistic.high_, statistic.scale_) + "," +
                    build_price(statistic.low_, statistic.scale_) + "," +
                    build_price(statistic.close_, statistic.scale_) + "," +
                    build_price(statistic.settle_, statistic.scale_) + "," +
                    boost::lexical_cast<std::string>(statistic.last_quantity_) + "\n";
        }
        else
        {
            msg = security_id + "," +
                    timestamp + "," +
                    build_price(statistic.last_, statistic.scale_) + "," +
                    boost::lexical_cast<std::string>(statistic.last_quantity_) + "\n";
        }
    }

    exchange_md_forward::exchange_md_forward(
            boost::asio::io_service& io,
            const std::string& internal_server_config,
            const std::string& local_service)
        : io_(io)
    {
        internal_server_manager_.reset(
                new internal_server_manager(io_, internal_server_config));

        client_manager_.reset(new client_manager(io_, local_service));
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
        // Convert data to security id
        const meta_data& m = *static_cast<const meta_data*>(data);

        // TODO...convert to string
        std::string msg;

        build_message(m._instrumentId, m._timestamp, statistic, msg, false);
        std::cout << msg << std::endl;
        client_manager_->send(m._instrumentId, msg);

        build_message(m._instrumentId, m._timestamp , statistic, msg, true);
        internal_server_manager_->send(m._instrumentId, msg);
        std::cout << msg << std::endl;
    }

}}} //tp::gateways::shfe
