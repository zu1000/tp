#include <comm/io/sender.hpp>

#include <boost/scoped_ptr.hpp>

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

    private:
        boost::scoped_ptr<comm::io::sender> stock_index_future_server_;
        boost::scoped_ptr<comm::io::sender> commodity_future_server_;
    };

}}} // tp::gateways::shfe
