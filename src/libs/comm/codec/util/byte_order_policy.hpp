#ifndef __tp_comm_codec_utils_byte_order_policy_hpp__
#define __tp_comm_codec_utils_byte_order_policy_hpp__

extern "C"
{
#include <stdint.h>
}

namespace tp {
namespace comm {
namespace codec {
namespace util {

    class byte_order_policy
    {
    public:
        static bool
        little_endian()
        { return little_endian_; }

        static uint8_t
        swap(uint8_t i)
        { return i; }

        static uint16_t
        swap(uint16_t i)
        {
            if (little_endian())
                return i = i << 8 | i >> 8;
            else
                return i;
        }

        static uint32_t
        swap(uint32_t i)
        {
            if (little_endian())
            {
                return swap(uint16_t(i & 0xffff)) << 16 |
                       swap(uint16_t(i >> 16));
            }
            else
                return i;
        }

        static uint64_t
        swap(uint64_t i)
        {
            if (little_endian())
            {
                return uint64_t(swap(uint32_t(i & 0xffffffff))) << 32 |
                       swap(uint32_t(i >> 32));
            }
            else
            {
                return i;
            }
        }

    private:
        static bool little_endian_;
    };

}}}} // tp::comm::codec::util

#endif//__tp_comm_codec_utils_byte_order_policy_hpp__
