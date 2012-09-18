#include <utils/number_to_string.hpp>

#include <boost/test/unit_test.hpp>

#include <cstring>
#include <sstream>

namespace tp {
namespace utils {
namespace test_number_to_string {

    BOOST_AUTO_TEST_CASE(UINT8_TO_STRING)
    {
        std::stringstream ss;

        uint8_t uint8 = 0;

        char res[20];

        std::memset(res, 0, sizeof(res));

        ss << (unsigned)uint8;

        BOOST_CHECK(uint8_to_string(uint8, res) == 1u);
        BOOST_CHECK(ss.str() == res);

        uint8 = 10;

        ss.str(""); ss << (unsigned)uint8;

        BOOST_CHECK(uint8_to_string(uint8, res) == 2u);
        BOOST_CHECK(ss.str() == res);

        uint8 = 0xff;

        ss.str(""); ss << (unsigned)uint8;
        BOOST_CHECK(uint8_to_string(uint8, res) == 3u);
        BOOST_CHECK(ss.str() == res);
    }

    BOOST_AUTO_TEST_CASE(INT8_TO_STRING)
    {
        std::stringstream ss;

        int8_t int8 = 0;

        char res[20];

        std::memset(res, 0, sizeof(res));

        ss << (int)int8;

        BOOST_CHECK(int8_to_string(int8, res) == 1u);
        BOOST_CHECK(ss.str() == res);

        int8 = 10;

        ss.str(""); ss << (int)int8;

        BOOST_CHECK(int8_to_string(int8, res) == 2u);
        BOOST_CHECK(ss.str() == res);

        int8 = -1;

        ss.str(""); ss << (int)int8;
        BOOST_CHECK(int8_to_string(int8, res) == 2u);
        BOOST_CHECK(ss.str() == res);

        int8 = 0x80;
        ss.str(""); ss << (int)int8;
        BOOST_CHECK(int8_to_string(int8, res) == 4u);
        BOOST_CHECK(ss.str() == res);

    }

    BOOST_AUTO_TEST_CASE(UNIT16_TO_STRING)
    {
        std::stringstream ss;

        uint16_t uint16 = 12345;

        char res[40];

        std::memset(res, 0, sizeof(res));

        ss << (unsigned)uint16;

        BOOST_CHECK(uint16_to_string(uint16, res) == 5u);
        BOOST_CHECK(ss.str() == res);
    }

    BOOST_AUTO_TEST_CASE(INT16_TO_STRING)
    {
        std::stringstream ss;

        int16_t int16 = 12345;

        char res[40];

        std::memset(res, 0, sizeof(res));

        ss << (int)int16;

        BOOST_CHECK(int16_to_string(int16, res) == 5u);
        BOOST_CHECK(ss.str() == res);

        int16 = -12345;

        ss.str(""); ss << (int)int16;
        BOOST_CHECK(int16_to_string(int16, res) == 6u);
        BOOST_CHECK(ss.str() == res);
    }

    BOOST_AUTO_TEST_CASE(UNIT32_TO_STRING)
    {
        std::stringstream ss;

        uint32_t uint32 = 1234567890;

        char res[40];

        std::memset(res, 0, sizeof(res));

        ss << (unsigned)uint32;

        BOOST_CHECK(uint32_to_string(uint32, res) == 10u);
        BOOST_CHECK(ss.str() == res);
    }

    BOOST_AUTO_TEST_CASE(INT32_TO_STRING)
    {
        std::stringstream ss;

        int32_t int32 = 1234567890;

        char res[40];

        std::memset(res, 0, sizeof(res));

        ss << (int)int32;

        BOOST_CHECK(int32_to_string(int32, res) == 10u);
        BOOST_CHECK(ss.str() == res);

        int32 = -1234567890;

        ss.str(""); ss << (int)int32;
        BOOST_CHECK(int32_to_string(int32, res) == 11u);
        BOOST_CHECK(ss.str() == res);
    }

    BOOST_AUTO_TEST_CASE(UNIT64_TO_STRING)
    {
        std::stringstream ss;

        uint64_t uint64 = 0xffffffff;
        uint64 = uint64 << 32 | 0xffffffff;

        char res[40];

        std::memset(res, 0, sizeof(res));

        ss << uint64;

        BOOST_CHECK(uint64_to_string(uint64, res) == 20u);
        BOOST_CHECK(ss.str() == res);
    }

    BOOST_AUTO_TEST_CASE(INT64_TO_STRING)
    {
        std::stringstream ss;

        // max negative integer
        int64_t int64 = 0x80000000;
        int64 = int64 << 32;

        char res[40];

        std::memset(res, 0, sizeof(res));

        ss << int64;

        BOOST_CHECK(int64_to_string(int64, res) == 20u);
        BOOST_CHECK(ss.str() == res);

        // positive
        int64 = -(int64+1);

        std::memset(res, 0, sizeof(res));

        ss.str(""); ss << int64;
        BOOST_CHECK(int64_to_string(int64, res) == 19u);
        BOOST_CHECK(ss.str() == res);
    }

    BOOST_AUTO_TEST_CASE(DOUBLE_TO_STRING)
    {
        double d = 0.0;

        char res[40];

        std::memset(res, 0, sizeof(res));

        BOOST_CHECK(double_to_string(d, res) == 3u);
        BOOST_CHECK(!std::strcmp(res, "0.0"));

        d = 1.1;

        BOOST_CHECK(double_to_string(d, res) == 3u);
        BOOST_CHECK(!std::strcmp(res, "1.1"));

        d = 1.001;
        BOOST_CHECK(double_to_string(d, res, 3) == 5u);
        BOOST_CHECK(!std::strcmp(res, "1.001"));
    }

}}} // tp::utils::test_number_to_string
