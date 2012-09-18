#include <utils/ring_buffer.hpp>
#include <utils/bucket.hpp>

#include <boost/test/unit_test.hpp>

#include <cstring>

namespace tp {
namespace utils {
namespace unittest {
namespace test_ring_buffer {

    typedef bucket<64>                  bucket_64;
    typedef ring_buffer<bucket_64, 20>  ring_buffer_64;

    BOOST_AUTO_TEST_CASE(BASIC)
    {
        ring_buffer_64 b;

        BOOST_CHECK(b.get() == NULL);

        char tmp[64*20 + 1];

        std::memset(tmp, 'T', sizeof(tmp));

        tmp[sizeof(tmp)-1] = 0;

        // copy less than a bucket size
        std::string str(tmp, 32);

        BOOST_CHECK(b.put(str.c_str()) == 32u);
        BOOST_CHECK(b.get() == NULL);

        b.flush();
        const ring_buffer_64::bucket_ptr_type& ptr1 = b.get();
        BOOST_CHECK(ptr1 != NULL);
        BOOST_CHECK(ptr1->space_left() == 32u);
        BOOST_CHECK(str == ptr1->data());

        // we consume the data and now there should not data available
        BOOST_CHECK(b.get() == NULL);

        BOOST_CHECK(b.put(str.c_str()) == 32u);
        BOOST_CHECK(b.get() == NULL);
        BOOST_CHECK(b.put(str.c_str()) == 32u);
        BOOST_CHECK(b.get() == NULL);
        BOOST_CHECK(b.put(str.c_str()) == 32u);
        const ring_buffer_64::bucket_ptr_type& ptr2 = b.get();
        BOOST_CHECK(ptr2->space_left() == 0u);

        b.flush();

        BOOST_CHECK(b.get()->space_left() == 32u);

        BOOST_CHECK(b.put(str.c_str(), str.length()) == str.length());
        BOOST_CHECK(b.get() == NULL);
        BOOST_CHECK(b.put(str.c_str(), str.length()) == str.length());
        BOOST_CHECK(b.get() == NULL);
        BOOST_CHECK(b.put(str.c_str(), str.length()) == str.length());
        BOOST_CHECK(b.get()->space_left() == 0u);

        b.flush();
        BOOST_CHECK(str == b.get()->data());

        str.assign(tmp, 128);
        BOOST_CHECK(b.put(str.c_str(), str.length()) == str.length());
        b.reset();
        BOOST_CHECK(b.put(str.c_str()) == 64u);
    }

    BOOST_AUTO_TEST_CASE(PUT_AN_GET)
    {
        ring_buffer_64 b;

        char tmp[65];
        tmp[64] = 0;

        std::memset(tmp, 'A', sizeof(tmp) - 1);
        std::string str1 = tmp;
        b.put(str1.c_str());
        std::memset(tmp, 'B', sizeof(tmp) - 1);
        std::string str2 = tmp;
        b.put(str2.c_str());
        std::memset(tmp, 'C', sizeof(tmp) - 1);
        std::string str3 = tmp;
        b.put(str3.c_str());
        
        b.flush();
        BOOST_CHECK(str1 == b.get()->data());
        BOOST_CHECK(str2 == b.get()->data());
        BOOST_CHECK(str3 == b.get()->data());
    }

}}}} //tp::utils::unittest
