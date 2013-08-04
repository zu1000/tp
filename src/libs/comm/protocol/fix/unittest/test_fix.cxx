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

        // should fail, since not finalized
        BOOST_CHECK_EQUAL(sb.get_length_from(11), 0);

        // Finalize the message
        sb.finalize();

        // Check the if we can get the correct length
        // 102=xxxxxxxxxx|11=xxxxx|131=x|10=xxx|
        //      14+1         8+1    5+1   6+1 = 37
        BOOST_CHECK_EQUAL(sb.get_length_from(102), 37);
        BOOST_CHECK_EQUAL(sb.get_length_from(11), 37 - 15);
        BOOST_CHECK_EQUAL(sb.get_length_from(131), 37 - 15 - 9);
        BOOST_CHECK_EQUAL(sb.get_length_from(10), 37 - 15 - 9 -6);

        // SET STRING
        char buff[200];
        memset(buff, 0, sizeof(buff));

        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        size_t chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value(102, "helloworld");
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value(102, "");
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value(102, "hello", 5, 0);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        // SET UNSIGNED INTEGER
        memset(buff, 0, sizeof(buff));
        sb.set_value(11, 1ull);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value(11, 13242ull);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value(11, 242ull, true);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        // SET INTEGER
        memset(buff, 0, sizeof(buff));
        sb.set_value(11, -1ll, true);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value(11, -256ll);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value(11, 1023ll);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        // Set Double
        memset(buff, 0, sizeof(buff));
        sb.set_value<2>(102, 2018.27, true);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value<2>(102, 20188.73);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value<2>(102, -10.73, true);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);

        memset(buff, 0, sizeof(buff));
        sb.set_value<4>(102, 10.7301, true);
        BOOST_CHECK_EQUAL(sb.copy_to(buff, sizeof(buff)), sb.space());
        std::cout << buff << std::endl;

        chksum = 0;
        for (size_t i = 0; i < sb.space() - 7; ++i)
            chksum += buff[i];
        BOOST_CHECK_EQUAL(sb.chksum(), chksum);
    }

}}}
