#include "acceptor.hpp"

namespace tp {
namespace comm {
namespace io {
namespace impl {

    acceptor::acceptor(
            acceptor_notifier& notifier)
        : notifier_(notifier)
    {}

    acceptor::~acceptor()
    {}

}}}} // tp::comm::io::impl
