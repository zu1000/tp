#include "policy.hpp"

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    std::size_t policy::check_message(const void* pkt, std::size_t& pkt_len)
    {
        if (!pkt)
            return 0;

        const header_t& header = *static_cast<const header_t*>(pkt);

        if (header.type_ >= MT_Unknown)
            return 0;

        pkt_len = header.size_;

        return 0;
    }

}}}} // tp::comm::protocol::internal
