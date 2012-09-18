#include <comm/protocol/internal/statistic.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace comm {
namespace protocol {
namespace internal {
namespace test_statistic {

    BOOST_AUTO_TEST_CASE(STATISTIC)
    {
        statistic_t s(1u);

        BOOST_CHECK_EQUAL(s.header_.type_, MT_Statistic);
        BOOST_CHECK_EQUAL(s.header_.size_, sizeof(statistic_t));
        BOOST_CHECK_EQUAL(s.security_id_, 1u);
    }

}}}}}//tp::comm::protocol::internal::test_statistic
