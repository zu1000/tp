#include <comm/service/service.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace comm {
namespace service {
namespace unittest {
namespace testservice {

    using namespace boost::asio;

    BOOST_AUTO_TEST_CASE(ipv4_ctor1)
    {
        const service_id id = 1;
        uint32_t port = 10086;
        bool     is_provider = false;
        const std::string  ip_v4 = "172.16.6.67";

        service s(id, service::TCP, ip_v4, port, is_provider);

        BOOST_CHECK_EQUAL(id, s.id());
        BOOST_CHECK_EQUAL(port, s.port());
        BOOST_CHECK_EQUAL(service::TCP, s.type());
        BOOST_CHECK_EQUAL(is_provider, s.is_provider());

        const ip::address& addr = s.addr();

        BOOST_CHECK(addr.is_v4());
        BOOST_CHECK(!addr.is_v6());

    }

    BOOST_AUTO_TEST_CASE(ipv6_ctor1)
    {
        const service_id id = 1;
        uint32_t port = 10086;
        bool     is_provider = true;
        const std::string ip_v6 = "fe80::20c:29ff:feb6:c8f7";

        service s(id, service::TCP, ip_v6, port, is_provider);

        BOOST_CHECK_EQUAL(id, s.id());
        BOOST_CHECK_EQUAL(port, s.port());
        BOOST_CHECK_EQUAL(service::TCP, s.type());
        BOOST_CHECK_EQUAL(is_provider, s.is_provider());

        const ip::address& addr = s.addr();

        BOOST_CHECK(!addr.is_v4());
        BOOST_CHECK(addr.is_v6());
    }

    BOOST_AUTO_TEST_CASE(ipv4_ctor2)
    {
        const service_id id = 1;
        uint32_t port = 10086;
        bool     is_provider = true;
        const ip::address& addr_v4 =
            ip::address::from_string("172.16.6.67");

        service s(id, service::TCP, addr_v4, port, is_provider);

        BOOST_CHECK_EQUAL(id, s.id());
        BOOST_CHECK_EQUAL(port, s.port());
        BOOST_CHECK_EQUAL(service::TCP, s.type());
        BOOST_CHECK_EQUAL(is_provider, s.is_provider());
    }

    BOOST_AUTO_TEST_CASE(ipv6_ctor2)
    {
        const service_id id = 1;
        uint32_t port = 10086;
        bool     is_provider = true;
        const ip::address& addr_v6 =
            ip::address::from_string("fe80::20c:29ff:feb6:c8f7");

        service s(id, service::TCP, addr_v6, port, is_provider);

        BOOST_CHECK_EQUAL(id, s.id());
        BOOST_CHECK_EQUAL(port, s.port());
        BOOST_CHECK_EQUAL(service::TCP, s.type());
        BOOST_CHECK_EQUAL(is_provider, s.is_provider());
    }

    BOOST_AUTO_TEST_CASE(validate_addr_type)
    {
        const service_id id = 1;
        uint32_t port = 10086;
        bool     is_provider = true;

        const std::string ip_v4 = "172.16.6.67";
        const std::string mc_v4 = "224.54.18.8";
        const std::string ip_v6 = "fe80::20c:29ff:feb6:c8f7";
        const std::string mc_v6 = "ff05::1:3";

        BOOST_CHECK_THROW(
                service(id, service::TCP, mc_v4, port, is_provider),
                std::logic_error);

        BOOST_CHECK_THROW(
                service(id, service::Multicast, ip_v4, port, is_provider),
                std::logic_error);

        BOOST_CHECK_THROW(
                service(id, service::TCP, mc_v6, port, is_provider),
                std::logic_error);

        BOOST_CHECK_THROW(
                service(id, service::Multicast, ip_v6, port, is_provider),
                std::logic_error);

    }

}}}}}//tp::comm::service::unittest::test::service
