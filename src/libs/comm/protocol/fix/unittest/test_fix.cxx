#include <comm/protocol/fix/fix.h>

#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <limits>

namespace blade { namespace fix { namespace test {

    BOOST_AUTO_TEST_CASE(UI_TO_STR)
    {
        char str[31];
        const char* res = ui_to_str(1234, str);
        BOOST_CHECK_EQUAL(std::string("1234"), std::string(res));

        uint64_t max_uint = std::numeric_limits<uint64_t>::max();
        res = ui_to_str(max_uint, str);
        BOOST_CHECK_EQUAL(
                boost::lexical_cast<std::string>(max_uint),
                std::string(res));
    }

    BOOST_AUTO_TEST_CASE(I_TO_STR)
    {
        char str[31];
        const char* res = i_to_str(-1234, str);
        BOOST_CHECK_EQUAL(std::string("-1234"), std::string(res));

        res = i_to_str(12345, str);
        BOOST_CHECK_EQUAL(std::string("12345"), std::string(res));

        res = i_to_str(2345, str);
        BOOST_CHECK_EQUAL(std::string("2345"), std::string(res));

        int64_t max_int = std::numeric_limits<int64_t>::max();
        res = i_to_str(max_int, str);
        BOOST_CHECK_EQUAL(
                boost::lexical_cast<std::string>(max_int),
                std::string(res));

        int64_t min_int = std::numeric_limits<int64_t>::min();
        res = i_to_str(min_int, str);
        BOOST_CHECK_EQUAL(
                boost::lexical_cast<std::string>(min_int),
                std::string(res));
    }

    BOOST_AUTO_TEST_CASE(DB_TO_STR)
    {
        char str[31];

        const char* res = db_to_str(0.001, 1, 3, str);
        BOOST_CHECK_EQUAL(std::string(res), "0.001");

        res = db_to_str(31.3827, 4, 6, str);
        BOOST_CHECK_EQUAL(std::string(res), "31.382700");

        res = db_to_str(-31.3827, 4, 6, str);
        BOOST_CHECK_EQUAL(std::string(res), "-31.382700");

        res = db_to_str(31327203.132, 10, 7, str);
        BOOST_CHECK_EQUAL(std::string(res), "31327203.1320000");

        res = db_to_str(0.0000001, 1, 7, str);
        BOOST_CHECK_EQUAL(std::string(res), "0.0000001");
    }

    BOOST_AUTO_TEST_CASE(UI_TO_STDSTR)
    {
        std::string res;

        ui_to_str(1234321, res);
        BOOST_CHECK_EQUAL(res, "1234321");

        ui_to_str(0, res);
        BOOST_CHECK_EQUAL(res, "0");
    }

    BOOST_AUTO_TEST_CASE(I_TO_STDSTR)
    {
        std::string res;
        i_to_str(-123123, res);
        BOOST_CHECK_EQUAL(res, "-123123");

        i_to_str(-1, res);
        BOOST_CHECK_EQUAL(res, "-1");

        i_to_str(0, res);
        BOOST_CHECK_EQUAL(res, "0");
    }

    BOOST_AUTO_TEST_CASE(FIELD)
    {
        field f(1002, 0, 10);

        BOOST_CHECK_EQUAL(f.tag_offset(), 0u);
        BOOST_CHECK_EQUAL(f.tag_size(), 5u);
        BOOST_CHECK_EQUAL(f.val_offset(), 5u);
        BOOST_CHECK_EQUAL(f.val_size(), 10u);
        BOOST_CHECK_EQUAL(f.space(), 16u);
        BOOST_CHECK_EQUAL(f.tag(), "1002=");
    }

    BOOST_AUTO_TEST_CASE(SIMPLE_BUILDER)
    {
        simple_builder<1024> sb;

        BOOST_CHECK_EQUAL(sb.push_tag(102, 10), true);
        BOOST_CHECK_EQUAL(sb.push_tag(11, 5), true);
        BOOST_CHECK_EQUAL(sb.push_tag(131, 1), true);

        // The push that should fail
        BOOST_CHECK_EQUAL(sb.push_tag(2, 0), false);
        BOOST_CHECK_EQUAL(sb.push_tag(11, 1), false);
        BOOST_CHECK_EQUAL(sb.push_tag(10, 1), false);
    }

}}}
