#include "../database.hpp"

#include <database/sqlite3_client.hpp>
#include <comm/protocol/internal/statistic.hpp>
#include <utils/number_to_string.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

using namespace tp::comm::protocol::internal;

namespace tp {
namespace database {
namespace ticker {
namespace unittest {

    const char* test_db_name = "test_ticker_db";

    struct tick_db_test_fixture
    {
        tick_db_test_fixture()
            : logger_(std::cout)
            , db_(new database(logger_))
            , test_statistic_(1u)
            , client_(test_db_name)
        {
            // init the test statistic info 
            test_statistic_.last_ = 1200;
            test_statistic_.last_quantity_ = 10000;
            test_statistic_.utc_timestamp_ = 121012287;
        }

        bool drop_table(const std::string& table_name)
        {
            return client_.table(sql_table(table_name, false));
        }

        tp::utils::simple_logger logger_;
        boost::scoped_ptr<database> db_;
        statistic_t test_statistic_;
        sqlite3_client client_;
    };

    BOOST_FIXTURE_TEST_CASE(open_ticker_db, tick_db_test_fixture)
    {
        BOOST_CHECK_EQUAL(true, db_->open(test_db_name));

        BOOST_CHECK_EQUAL(true, db_->update_tick(test_statistic_));

        // destory it to save all updates
        db_.reset();

        BOOST_CHECK_EQUAL(true, drop_table("1"));
    }

    BOOST_FIXTURE_TEST_CASE(add_ticker_db, tick_db_test_fixture)
    {
        BOOST_CHECK_EQUAL(true, db_->open(test_db_name));

        for (uint64_t i = 0; i < 100; ++i)
        {
            BOOST_CHECK_EQUAL(true, db_->update_tick(test_statistic_));
            test_statistic_.security_id_++;
        }

        db_.reset();

        client_.begin_transaction();
        for (uint64_t i = 0; i < 100; ++i)
        {
            char num[10] = {0};
            BOOST_CHECK_EQUAL(true, (utils::int64_to_string(i+1, num) > 0));
            BOOST_CHECK_EQUAL(true, drop_table(num));
        }
        client_.end_transaction();
    }

    BOOST_FIXTURE_TEST_CASE(add_1000_ticker_db, tick_db_test_fixture)
    {
        BOOST_CHECK_EQUAL(true, db_->open(test_db_name));

        for (uint64_t i = 0; i < 1000; ++i)
        {
            BOOST_CHECK_EQUAL(true, db_->update_tick(test_statistic_));
            test_statistic_.security_id_++;
            usleep(10);
        }

        db_.reset();

        client_.begin_transaction();
        for (uint64_t i = 0; i < 1000; ++i)
        {
            char num[10] = {0};
            BOOST_CHECK_EQUAL(true, (utils::int64_to_string(i+1, num) > 0));
            BOOST_CHECK_EQUAL(true, drop_table(num));
        }
        client_.end_transaction();
    }

    BOOST_FIXTURE_TEST_CASE(add_1000_ticker_db2, tick_db_test_fixture)
    {
        BOOST_CHECK_EQUAL(true, db_->open(test_db_name));

        for (uint64_t i = 0; i < 1000; ++i)
        {
            BOOST_CHECK_EQUAL(true, db_->update_tick(test_statistic_));
            test_statistic_.last_++;
            usleep(10);
        }

        db_.reset();

        BOOST_CHECK_EQUAL(true, drop_table("1"));
    }

}}}} // tp::database::ticker::unittest
