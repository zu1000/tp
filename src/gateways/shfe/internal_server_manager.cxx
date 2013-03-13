#include "internal_server_manager.hpp"

#include "utils.hpp"

#include <comm/service/service.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    internal_server_manager::internal_server_manager(
            boost::asio::io_service& io,
            const std::string& config_file)
        : stock_timer_(io)
        , comm_timer_(io)
        , io_(io)
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

        stock_.reset(
                new comm::service::service(
                    -1,
                    comm::service::service::TCP,
                    stock_host,
                    stock_port,
                    false)
                );
        comm_.reset(
                new comm::service::service(
                    -1,
                    comm::service::service::TCP,
                    comm_host,
                    comm_port,
                    false)
                );

        connect(true);
        connect(false);
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

    std::size_t internal_server_manager::received(
            const comm::io::const_buffer& buffer,
            std::size_t size, bool is_stock)
    {
        boost::shared_ptr<comm::io::sender_receiver> server =
            is_stock ? stock_index_future_server_ : commodity_future_server_;

        // Echo back the data we received
        comm::io::error_code error;
        server->send(buffer, error);
        return size;
    }

    bool internal_server_manager::error_occurred(
            const comm::io::error_code& ec, bool is_stock)
    {
        boost::shared_ptr<comm::io::sender_receiver> server =
            is_stock ? stock_index_future_server_ : commodity_future_server_;

        // Need to start reconnection
        std::cout << "ERROR occurred for " << (is_stock ? "stock" : "commodity")
                  << " server" << std::endl;

        std::cout << "Reset and reconnect the servers" << std::endl;

        connect(is_stock, ec);

        return false;
    }

    bool internal_server_manager::connect(
            bool is_stock,
            const comm::io::error_code& ec)
    {
        std::string name = is_stock ? " Stock Index " : " Commodity Index ";
        try
        {
            boost::shared_ptr<comm::io::sender_receiver>& server =
                is_stock ? stock_index_future_server_ : commodity_future_server_;

            std::cout << "Connecting" << name << "server" << std::endl;

            server.reset(
                    new comm::io::sender_receiver(
                        (is_stock ? *stock_ : *comm_), io_,
                        boost::bind(&internal_server_manager::received,
                                    this, _1, _2, is_stock),
                        boost::bind(&internal_server_manager::error_occurred,
                                    this, _1, is_stock)));
        }
        catch(boost::system::system_error& e)
        {
            boost::asio::deadline_timer& timer =
                is_stock ? stock_timer_ : comm_timer_;
            std::cout << "Connect to" << name
                      << "server failed, will reconnect in 2 sec"
                      << std::endl;
            timer.expires_from_now(boost::posix_time::seconds(2));
            timer.async_wait(
                    boost::bind(&internal_server_manager::connect,
                                this, is_stock, _1));
        }
    }

}}} // tp::gateways::shfe
