#include <utils/bucket.hpp>

#include <boost/test/unit_test.hpp>

#include <cstring>

namespace tp {
namespace utils {
namespace unittest {
namespace test_bucket {

    typedef bucket<64> bucket_64b;

    BOOST_AUTO_TEST_CASE(BASIC)
    {
        bucket_64b b;

        BOOST_CHECK(b.index() == -1);
        BOOST_CHECK(b.size() == 64u);
        BOOST_CHECK(b.space_left() == 64u);

        std::string str = "Hello world";
        b.copy(str.c_str(), str.length());

        BOOST_CHECK(b.space_left() == (b.size() - str.length()));
        BOOST_CHECK(str == b.data());

        b.copy(str.c_str(), str.length());
        BOOST_CHECK(b.space_left() == (b.size() - 2*str.length()));

        b.copy(str.c_str());
        BOOST_CHECK(b.space_left() == (b.size() - 3*str.length()));

        str = str+str+str;
        BOOST_CHECK(str == b.data());
    }

    BOOST_AUTO_TEST_CASE(OVERSIZED_DATA)
    {
        bucket_64b b(1);

        BOOST_CHECK(b.index() == 1);

        char tmp[256];
        std::memset(tmp, 'T', 256);
        tmp[255] = 0;

        std::string str(tmp, 64);

        size_t size = b.copy(tmp);
        BOOST_CHECK(b.size() == size);
        BOOST_CHECK(b.space_left() == 0u);
        BOOST_CHECK(str == b.data());

        b.reset_space_left();
        BOOST_CHECK(b.space_left() == 64u);
        BOOST_CHECK(str == b.data());

        b.reset();
        BOOST_CHECK(b.space_left() == 64u);
        BOOST_CHECK(b.data()[0] == 0u);

        size = b.copy(tmp, sizeof(tmp));
        BOOST_CHECK(b.size() == size);
        BOOST_CHECK(b.space_left() == 0u);
        BOOST_CHECK(str == b.data());
    }

}}}}//tp::utils::unittest::test_bucket
