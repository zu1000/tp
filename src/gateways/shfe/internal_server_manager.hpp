#include <comm/io/sender_receiver.hpp>

#include <boost/shared_ptr.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    class internal_server_manager
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

    private:
        boost::shared_ptr<comm::io::sender_receiver> stock_index_future_server_;
        boost::shared_ptr<comm::io::sender_receiver> commodity_future_server_;
    };

}}} // tp::gateways::shfe
