#include "ostream_logger_server.hpp"

#include <iostream>

namespace tp {
namespace utils {

    typedef ostream_logger_server<0xffff, 100> ostream_logger_server_64k_100;
    ostream_logger_server_64k_100 ols(std::cout);

}} // tp::utils
