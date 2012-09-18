#include "simple_logger.hpp"

namespace tp {
namespace utils {

    simple_logger::simple_logger(std::ostream& os)
        : server_(os)
        , client_(server_)
    {}

    simple_logger::~simple_logger()
    {
        client_.flush();
    }

}} // tp::utils
