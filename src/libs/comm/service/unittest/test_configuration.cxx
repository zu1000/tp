#include <comm/service/configuration.hpp>

#include <boost/test/unit_test.hpp>
#include <sstream>

using namespace boost::asio;

namespace tp {
namespace comm {
namespace service {
namespace test_configuration {

    BOOST_AUTO_TEST_CASE(config_with_throw)
    {
        std::string input = "";
        std::istringstream iss(input);

        BOOST_CHECK_THROW(new configuration(iss), std::runtime_error);
    }

    BOOST_AUTO_TEST_CASE(config_with_serive_to_subscribe)
    {
        std::string input =
            "<service owner=\"n\" id=1 />"
            "<service owner=\"m\" id=2 type=\"TCP\" addr=\"182.16.6.1\" port=1928 />";
        std::istringstream iss(input);

        configuration config(iss);

        BOOST_CHECK_EQUAL(0u, config.get_service_to_publish().size());
        BOOST_CHECK_EQUAL(1u, config.get_service_to_subscribe().size());

        const service_id_set& ids = config.get_service_to_subscribe();
        BOOST_CHECK_EQUAL(1u, *ids.begin());
    }

    BOOST_AUTO_TEST_CASE(config_with_serive_to_publish)
    {
        std::string input =
            "<service owner=\"y\" id=1 type=\"TCP\" addr=\"182.16.6.1\" port=1928 />"
            "<service owner=\"m\" id=2 type=\"TCP\" addr=\"182.16.6.2\" port=1929 />";
        std::istringstream iss(input);

        configuration config(iss);

        BOOST_CHECK_EQUAL(1u, config.get_service_to_publish().size());
        BOOST_CHECK_EQUAL(0u, config.get_service_to_subscribe().size());

        const service_id_to_ptr_map::const_iterator& it =
            config.get_service_to_publish().begin();

        BOOST_CHECK_EQUAL(1u, it->first);
        BOOST_CHECK_EQUAL(1u, it->second->id());
        BOOST_CHECK_EQUAL(service::TCP, it->second->type());
        BOOST_CHECK_EQUAL(1928u, it->second->port());
        BOOST_CHECK_EQUAL(
                ip::address::from_string("182.16.6.1"), it->second->addr());
        BOOST_CHECK_EQUAL(true, it->second->is_provider());
    }

    BOOST_AUTO_TEST_CASE(config_with_multi_services)
    {
        std::string input =
            "<service owner=\"n\" id=1 />"
            "<service owner=\"n\" id=2 />"
            "<service owner=\"y\" id=3 type=\"TCP\" addr=\"182.16.6.1\" port=1928 />"
            "<service owner=\"y\" id=6 type=\"TCP\" addr=\"182.16.6.7\" port=1930 />"
            "<service owner=\"y\" id=4 type=\"TCP\" addr=\"182.16.6.2\" port=1929 />"
            "<service owner=\"m\" id=7 type=\"TCP\" addr=\"182.16.6.6\" port=1931 />";
        std::istringstream iss(input);

        configuration config(iss);
        BOOST_CHECK_EQUAL(3u, config.get_service_to_publish().size());
        BOOST_CHECK_EQUAL(2u, config.get_service_to_subscribe().size());
    }

    BOOST_AUTO_TEST_CASE(config_wrong)
    {
        std::string input = "<service owner=\"n\" id=1/>";
        std::istringstream iss(input);

        boost::scoped_ptr<configuration> config;

        BOOST_CHECK_THROW(
                config.reset(new configuration(iss)), std::runtime_error);
    }

    BOOST_AUTO_TEST_CASE(service_manager_configuration)
    {
        std::string input =
            "<service owner=\"m\" id=3 type=\"TCP\" addr=\"182.16.6.1\" port=1928 />";
        std::istringstream iss(input);

        configuration config(iss);
        BOOST_CHECK_EQUAL(0u, config.get_service_to_publish().size());
        BOOST_CHECK_EQUAL(0u, config.get_service_to_subscribe().size());

        const service& s = config.get_service_manager_configuration();
        BOOST_CHECK_EQUAL(3u, s.id());
        BOOST_CHECK_EQUAL(service::TCP, s.type());
        BOOST_CHECK_EQUAL(1928u, s.port());
        BOOST_CHECK_EQUAL(
                ip::address::from_string("182.16.6.1"), s.addr());
        BOOST_CHECK_EQUAL(true, s.is_provider());
    }

}}}} // tp::comm::service::test_configuration
