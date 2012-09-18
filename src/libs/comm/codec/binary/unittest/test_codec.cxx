#include <comm/codec/binary/codec.hpp>

#include <boost/test/unit_test.hpp>
#include <vector>

namespace tp {
namespace comm {
namespace codec {
namespace binary {
namespace unittest {

    enum message_types
    {
        test_type1,
        test_type2,
        unknown
    };

    struct m_header
    {
        m_header(message_types type)
            : length_(0)
            , type_(type)
        {}

        unsigned length_;
        message_types type_;
    };

    struct type1
    {
        type1()
            : h_(test_type1)
        {
            h_.length_= sizeof(type1);
        }

        m_header h_;
        unsigned m1_;
    };

    struct type2
    {
        type2()
            : h_(test_type2)
        {
            h_.length_ = sizeof(type1);
        }

        m_header h_;
        unsigned m1_;
        unsigned m2_;
    };

    struct codec_test_fixture
    {
        typedef m_header msg_header_type;
        typedef codec_test_fixture msg_handler_type;
        typedef codec<codec_test_fixture> codec_type;

        codec_test_fixture()
            : codec_(*this)
        {
        }

        static const std::size_t min_pkt_len = sizeof(unsigned);

        static std::size_t check_message(const void* ph, std::size_t& pkt_len)
        {
            pkt_len = *static_cast<const unsigned*>(ph);
            return sizeof(unsigned);
        }

        std::size_t handle_message(const m_header* msg)
        {
            switch (msg->type_)
            {
            case test_type1:
                v1_.push_back(*reinterpret_cast<const type1*>(msg));
                return sizeof(type1);
            case test_type2:
                v2_.push_back(*reinterpret_cast<const type2*>(msg));
                return sizeof(type2);
            default:
                return 0;
            }
        }

        codec_type codec_;
        char buffer_[1000];
        std::vector<type1> v1_;
        std::vector<type2> v2_;
    };

    BOOST_FIXTURE_TEST_CASE(codec_single, codec_test_fixture)
    {
        type1 t1; t1.m1_ = 1u;

        std::size_t size = 0u;
        unsigned pkt_len = sizeof(type1) + sizeof(unsigned); 


        boost::asio::mutable_buffer encode_buf(buffer_, sizeof(buffer_));
        size += codec_.encode(pkt_len, encode_buf); 
        size += codec_.encode(t1, encode_buf);
        BOOST_CHECK_EQUAL(pkt_len, size);

        boost::asio::const_buffer decode_buf(buffer_, size);
        size = codec_.decode(decode_buf, size);
        BOOST_CHECK_EQUAL(pkt_len, size);

        BOOST_CHECK_EQUAL(1u, v1_.size());
        BOOST_CHECK_EQUAL(v1_[0].m1_, 1u);
    }

    BOOST_FIXTURE_TEST_CASE(codec_multiple, codec_test_fixture)
    {
        type1 t11, t12;
        t11.m1_ = 1u;
        t12.m1_ = 2u;

        type2 t21, t22, t23;
        t21.m1_ = 11u; t21.m2_ = 22u;
        t22.m1_ = 33u; t22.m2_ = 44u;
        t23.m1_ = 55u; t23.m2_ = 66u;

        unsigned pkt_len = 2*sizeof(type1) + 3*sizeof(type2) + sizeof(unsigned);
        size_t size = 0u;

        boost::asio::mutable_buffer encode_buf(buffer_, sizeof(buffer_));
        size += codec_.encode(pkt_len, encode_buf);
        size += codec_.encode(t11, encode_buf);
        size += codec_.encode(t21, encode_buf);
        size += codec_.encode(t22, encode_buf);
        size += codec_.encode(t12, encode_buf);
        size += codec_.encode(t23, encode_buf);
        BOOST_CHECK_EQUAL(pkt_len, size);

        boost::asio::const_buffer decode_buf(buffer_, size);
        size = codec_.decode(decode_buf, size);
        BOOST_CHECK_EQUAL(pkt_len, size);

        BOOST_CHECK_EQUAL(2u, v1_.size());
        BOOST_CHECK_EQUAL(3u, v2_.size());

    }

}}}}} // tp::comm::codec::binary::unittest
