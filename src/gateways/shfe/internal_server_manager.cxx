#include "internal_server_manager.hpp"

#include "utils.hpp"

#include <comm/service/service.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    internal_server_manager::internal_server_manager(
            boost::asio::io_service& io,
            const std::string& config_file)
    {
        std::string stock_host, comm_host;
        unsigned short stock_port=0, comm_port=0;

        const char* stock_index_future_host = "stock_index_future_host";
        const char* commodity_future_host   = "commodity_future_host";
        const char* stock_index_future_port = "stock_index_future_port";
        const char* commodity_future_port   = "commodity_future_port";

        application::options options("internal");

        add_option<std::string>(options, stock_index_future_host);
        add_option<std::string>(options, commodity_future_host);
        add_option<unsigned short>(options, stock_index_future_port);
        add_option<unsigned short>(options, commodity_future_port);

        if (!options.parse(config_file))
            throw std::invalid_argument("Invalid internal server config");

        get_mandatory_option(
                options, stock_index_future_host, stock_host);
        get_mandatory_option(
                options, stock_index_future_port, stock_port);
        get_mandatory_option(
                options, commodity_future_host, comm_host);
        get_mandatory_option(
                options, commodity_future_port, comm_port);

        comm::service::service stock(
                -1,
                comm::service::service::TCP,
                stock_host,
                stock_port,
                false);
        comm::service::service comm(
                -1,
                comm::service::service::TCP,
                comm_host,
                comm_port,
                false);
        stock_index_future_server_.reset(new comm::io::sender(stock, io));
        commodity_future_server_.reset(new comm::io::sender(comm, io));
    }

    void internal_server_manager::send(
            const std::string& security_id, const std::string& info)
    {
        bool index_future = security_id.substr(0, 2) == "jf";
        comm::io::error_code error;
        if (stock_index_future_server_ && index_future)
            stock_index_future_server_->send(
                    comm::io::const_buffer(info.c_str(), info.size()), error);

        if (commodity_future_server_ && !index_future)
            commodity_future_server_->send(
                    comm::io::const_buffer(info.c_str(), info.size()), error);
    }

}}} // tp::gateways::shfe
