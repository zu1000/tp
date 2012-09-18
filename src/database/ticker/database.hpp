#ifndef __tp_database_ticker_database_hpp__
#define __tp_database_ticker_database_hpp__

#include <utils/simple_logger.hpp>
#include <database/client.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    struct statistic_t;

}}}}

namespace tp {
namespace database {
namespace ticker {

    class database
    {
    public:
        database(
                tp::utils::simple_logger& logger);

        ~database();

        bool open(
                const std::string name);

        bool update_tick(
                const tp::comm::protocol::internal::statistic_t& statistic);

    private:
        void batch_updates();
        void stop();

    private:
        boost::scoped_ptr<client> client_;
        tp::utils::simple_logger& logger_;
    };

}}} // tp::database::indicator
#endif//__tp_database_indicator_database_hpp__
