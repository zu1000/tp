#include <utils/ostream_logger_server.hpp>

#include <boost/test/unit_test.hpp>
#include <sstream>

namespace tp {
namespace utils {
namespace test_ostream_logger_server {

    typedef ostream_logger_server<64, 10> ostream_logger_server_64_100;

    BOOST_AUTO_TEST_CASE(SHORT_STRING_AND_QUIT)
    {
        std::stringstream ss;
        std::string test_str = "test me";

        {
            ostream_logger_server_64_100 osl(ss);
            osl.write(test_str.c_str());
        }

        BOOST_CHECK(ss.str() == test_str);
    }

    BOOST_AUTO_TEST_CASE(LONG_STRING_WITH_SIZE_AND_QUIT)
    {
        std::stringstream ss;
        std::string test_str(64*3, 'T');

        {
            ostream_logger_server_64_100 osl(ss);
            osl.write(test_str.c_str(), test_str.length());
        }

        BOOST_CHECK(ss.str() == test_str);
    }

    BOOST_AUTO_TEST_CASE(LONG_STRING_NO_SIZE_AND_QUIT)
    {
        std::stringstream ss;
        std::string test_str(66, 'T');

        {
            ostream_logger_server_64_100 osl(ss);
            osl.write(test_str.c_str());
        }

        BOOST_CHECK(ss.str() == test_str.substr(0, 64));
    }

}}} // tp::utils::test_ostream_logger_server
