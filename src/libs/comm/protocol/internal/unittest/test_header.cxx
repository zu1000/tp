#include <comm/protocol/internal/header.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace comm {
namespace protocol {
namespace internal {
namespace test_header {

    BOOST_AUTO_TEST_CASE(HEADER)
    {
        header_t s(10u, MT_AddService);

        BOOST_CHECK_EQUAL(INTERNAL_PROTOCOL_VERSION, s.version_);
        BOOST_CHECK_EQUAL(10u, s.size_);
        BOOST_CHECK_EQUAL(MT_AddService, s.type_);
    }

}}}}}
