#include <comm/protocol/internal/market.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace comm {
namespace protocol {
namespace internal {
namespace test_market {

    BOOST_AUTO_TEST_CASE(MARKET)
    {
        market_t market(1u);

        BOOST_CHECK_EQUAL(market.header_.type_, MT_Market);
        BOOST_CHECK_EQUAL(
                market.header_.size_,
                sizeof(market_t) - market_t::array_size*sizeof(market_level_t));
        BOOST_CHECK_EQUAL(market.security_id_, 1u);
    }

}}}}} // tp::comm::protocol::internal::test_market
