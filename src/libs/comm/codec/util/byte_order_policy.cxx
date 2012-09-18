#include "byte_order_policy.hpp"

namespace tp {
namespace comm {
namespace codec {
namespace util {

static union {
    uint32_t number;
    uint8_t  raw[sizeof(uint32_t)];
} test = {1UL};

bool
byte_order_policy::little_endian_ = (test.raw[0] & 0x1);

}}}} // tp::comm::codec::util
