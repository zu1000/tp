#include <application/options.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace application {
namespace unittest {

    BOOST_AUTO_TEST_CASE(test_ctor)
    {
        options opt("test_option");

        opt.add(
                "long_name",
                "S",
                boost::optional<int>(),
                "test description");

        int value = 0;
        BOOST_CHECK_EQUAL(false, opt.get("long_name", value));
    }

    BOOST_AUTO_TEST_CASE(test_get_options)
    {
        options opt("test_option");

        opt.add(
                "long_name",
                "S",
                boost::optional<int>(),
                "test description");

        const char* args[] =
        {
            "test_prg",
            "--long_name=1",
            0
        };

        opt.parse(2u, args);

        int value = 0;
        BOOST_CHECK_EQUAL(true, opt.get("long_name", value));
        BOOST_CHECK_EQUAL(1, value);
    }

    BOOST_AUTO_TEST_CASE(test_options_shortcut)
    {
        options opt("test_option");

        opt.add(
                "long_name",
                "S",
                boost::optional<int>(),
                "test description");

        const char* args[] =
        {
            "test_prg",
            "-S1",
            0
        };

        BOOST_CHECK_EQUAL(true, opt.parse(2u, args));

        int value = 0;
        BOOST_CHECK_EQUAL(true, opt.get("long_name", value));
        BOOST_CHECK_EQUAL(1, value);
    }

    BOOST_AUTO_TEST_CASE(test_options_shortcut2)
    {
        options opt("test_option");

        opt.add(
                "long_name",
                "S",
                boost::optional<int>(),
                "test description");

        const char* args[] =
        {
            "test_prg",
            "-S",
            "3",
            0
        };

        BOOST_CHECK_EQUAL(true, opt.parse(3u, args));

        int value = 0;
        BOOST_CHECK_EQUAL(true, opt.get("long_name", value));
        BOOST_CHECK_EQUAL(3, value);
    }

    BOOST_AUTO_TEST_CASE(test_options_with_default_value)
    {
        options opt("test_option");

        opt.add(
                "long_name",
                "S",
                boost::optional<int>(-1),
                "test description");

        const char* args[] =
        {
            "test_prg",
            0
        };

        BOOST_CHECK_EQUAL(true, opt.parse(1u, args));

        int value = 0;
        BOOST_CHECK_EQUAL(true, opt.get("long_name", value));
        BOOST_CHECK_EQUAL(-1, value);
    }

}}} // tp::application::unittest
