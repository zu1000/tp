#include "internal_server_manager.hpp"

#include "utils.hpp"

#include <comm/service/service.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    internal_server_manager::internal_server_manager(
            boost::asio::io_service& io,
            tp::application::options& options)
    {
        std::string stock_host, comm_host;
        short stock_port, comm_port;

        get_mandatory_option(
                options, "stock_index_future_host", stock_host);
        get_mandatory_option(
                options, "stock_index_future_port", stock_port);
        get_mandatory_option(
                options, "commodity_future_host", comm_host);
        get_mandatory_option(
                options, "commodity_future_port", comm_host);

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

    void internal_server_manager::send(const std::string& info)
    {
        comm::io::error_code error;
        if (stock_index_future_server_)
            stock_index_future_server_->send(
                    comm::io::const_buffer(info.c_str(), info.size()), error);

        if (commodity_future_server_)
            commodity_future_server_->send(
                    comm::io::const_buffer(info.c_str(), info.size()), error);
    }

}}} // tp::gateways::shfe
