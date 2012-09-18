#include <comm/io/sender.hpp>
#include <application/options.hpp>

#include <boost/scoped_ptr.hpp>

namespace tp {
namespace gateways {
namespace shfe {

    class internal_server_manager
    {
    public:
        internal_server_manager(
                boost::asio::io_service& io,
                tp::application::options& options);

        void send(const std::string& info);

    private:
        boost::scoped_ptr<comm::io::sender> stock_index_future_server_;
        boost::scoped_ptr<comm::io::sender> commodity_future_server_;
    };

}}} // tp::gateways::shfe
