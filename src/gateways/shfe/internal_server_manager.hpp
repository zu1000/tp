#include <comm/io/sender_receiver.hpp>

#include <boost/asio/deadline_timer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    class internal_server_manager
        : public boost::enable_shared_from_this<internal_server_manager>
    {
    public:
        internal_server_manager(
                boost::asio::io_service& io,
                const std::string& config_file);

        void send(const std::string& security_id, const std::string& info);

        std::size_t received(
                const comm::io::const_buffer& buffer,
                std::size_t size, bool is_stock);

        bool error_occurred(
                const comm::io::error_code& ec, bool is_stock);

        bool connect(
                bool is_stock,
                const comm::io::error_code& ec=comm::io::error_code());

    private:
        boost::shared_ptr<comm::io::sender_receiver> stock_index_future_server_;
        boost::shared_ptr<comm::io::sender_receiver> commodity_future_server_;
        boost::scoped_ptr<comm::service::service> stock_;
        boost::scoped_ptr<comm::service::service> comm_;
        boost::asio::deadline_timer stock_timer_;
        boost::asio::deadline_timer comm_timer_;
        boost::asio::io_service& io_;
    };

}}} // tp::gateways::shfe
