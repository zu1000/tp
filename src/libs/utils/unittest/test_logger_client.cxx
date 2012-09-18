#include <utils/logger_client.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace utils {
namespace unittest {
namespace test_logger_client {

    struct test_ostream_logger
    {
        void write(const char* msg)
        {
            size_t len = std::strlen(msg);
            data_.append(msg, len);
        }

        void write(const char* msg, size_t len)
        {
            data_.append(msg, len);
        }

        void flush()
        {
            data_.clear();
        }

        std::string data_;
    };

    typedef logger_client<test_ostream_logger> test_logger;

    struct test_fixture
    {
        test_fixture()
            : server_()
            , logger_(server_)
        {}

        test_ostream_logger server_;
        test_logger         logger_;
    };

    BOOST_FIXTURE_TEST_CASE(logcharstring, test_fixture)
    {
        std::string test_data = "logcharstring";
        logger_ << test_data.c_str();

        BOOST_CHECK_EQUAL(server_.data_, test_data);
    }

    BOOST_FIXTURE_TEST_CASE(logstring, test_fixture)
    {
        std::string test_data = "logstring";
        logger_ << test_data;

        BOOST_CHECK_EQUAL(server_.data_, test_data);
    }

    BOOST_FIXTURE_TEST_CASE(logchar, test_fixture)
    {
        char test_data = 'c';
        logger_ << test_data;

        BOOST_CHECK_EQUAL(server_.data_, "c");
    }

    BOOST_FIXTURE_TEST_CASE(logint8, test_fixture)
    {
        int8_t min = 0x80, max = 0x7f;
        std::stringstream int8_min, int8_max;

        int8_min << int(min);
        int8_max << int(max);

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, int8_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, int8_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(loguint8, test_fixture)
    {
        uint8_t min = 0x00, max = 0xff;
        std::stringstream uint8_min, uint8_max;

        uint8_min << int(min);
        uint8_max << int(max);

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, uint8_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, uint8_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(logint16, test_fixture)
    {
        int16_t min = 0x8000, max = 0x7fff;
        std::stringstream int16_min, int16_max;

        int16_min << min;
        int16_max << max;

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, int16_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, int16_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(loguint16, test_fixture)
    {
        uint16_t min = 0, max = 0xffff;
        std::stringstream uint16_min, uint16_max;

        uint16_min << min;
        uint16_max << max;

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, uint16_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, uint16_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(logint32, test_fixture)
    {
        int32_t min = 0x80000000, max = 0x7fffffff;
        std::stringstream int32_min, int32_max;

        int32_min << min;
        int32_max << max;

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, int32_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, int32_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(loguint32, test_fixture)
    {
        uint32_t min = 0, max = 0xffffffff;
        std::stringstream uint32_min, uint32_max;

        uint32_min << min;
        uint32_max << max;

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, uint32_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, uint32_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(logint64, test_fixture)
    {
        int64_t min = 1, max;
        min <<= 63;
        max = ~min;

        std::stringstream int64_min, int64_max;

        int64_min << min;
        int64_max << max;

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, int64_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, int64_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(loguint64, test_fixture)
    {
        uint64_t min = 0, max = ~min;
        std::stringstream uint64_min, uint64_max;

        uint64_min << min;
        uint64_max << max;

        logger_ << min;
        BOOST_CHECK_EQUAL(server_.data_, uint64_min.str());

        logger_.flush();

        logger_ << max;
        BOOST_CHECK_EQUAL(server_.data_, uint64_max.str());
    }

    BOOST_FIXTURE_TEST_CASE(logdouble, test_fixture)
    {
        double d = 1.5;
        std::stringstream d_str;
        
        d_str << d;

        logger_ << d;
        BOOST_CHECK_EQUAL(server_.data_, d_str.str());
    }

    BOOST_FIXTURE_TEST_CASE(log_combination, test_fixture)
    {
        std::stringstream ss;

        ss << "I" << " " << "like" << " " << 124 << " " << "not " << 124.5;

        logger_ << "I" << " " << "like" << " " << 124 << " " << "not " << 124.5;

        BOOST_CHECK_EQUAL(ss.str(), server_.data_);
    }

}}}} // tp::utils::unittest::test_logger_client
