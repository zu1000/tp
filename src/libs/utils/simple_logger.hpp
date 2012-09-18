#ifndef __tp_utils_simple_logger_hpp__
#define __tp_utils_simple_logger_hpp__

#include "ostream_logger_server.hpp"
#include "logger_client.hpp"
#include <iostream>

namespace tp {
namespace utils {


    class simple_logger
    {
    public:
        typedef ostream_logger_server<0xffff, 100> server_type;
        typedef logger_client<server_type>         client_type;

        simple_logger(std::ostream& os);
        ~simple_logger();

        template <typename T>
        client_type& operator <<(const T& msg)  { return client_ << msg; }

        bool flush()                    { return client_.flush(); }
    private:
        server_type server_;
        client_type client_;
    };

}} // tp::utils

#endif//__tp_utils_simple_logger_hpp__
