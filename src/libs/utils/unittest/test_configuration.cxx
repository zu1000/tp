#include <utils/configuration.hpp>

#include <boost/test/unit_test.hpp>
#include <sstream>

namespace tp {
namespace utils {
namespace unittest {
namespace test_configuration{

    BOOST_AUTO_TEST_CASE(single_option)
    {
        std::string input = "<option1>value1</option1>";
        std::istringstream iss(input);

        configuration config(iss);

        std::string value = config.get("option1");

        BOOST_CHECK_EQUAL(value, "value1");
    }

    BOOST_AUTO_TEST_CASE(multi_options)
    {
        std::string input =
            "<option1>value1</option1>\n<option2>value2</option2>";
        std::istringstream iss(input);

        configuration config(iss);

        std::string value1 = config.get("option1");
        std::string value2 = config.get("option2");
        std::string value3 = config.get("option3");
        BOOST_CHECK_EQUAL(value1, "value1");
        BOOST_CHECK_EQUAL(value2, "value2");
        BOOST_CHECK_EQUAL(value3, "");
    }

}}}} // tp::utils::unittest::test_configuration
