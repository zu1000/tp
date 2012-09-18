#ifndef __tp_utils_logger_client_hpp__
#define __tp_utils_logger_client_hpp__

#include "number_to_string.hpp"

#include <comm/service/id.hpp>

#include <boost/noncopyable.hpp>

#include <ostream>
#include <string>

extern "C"
{
#include <stdint.h>
}

namespace tp {
namespace utils {

    static const size_t buffer_size = 0xffff;

    template <typename L>
    class logger_client : public boost::noncopyable
    {
    public:
        typedef L logger_server_type;
        logger_client(logger_server_type& logger_server)
            : logger_server_(logger_server)
        {}

        logger_client& operator << (const char* msg);
        logger_client& operator << (const std::string& msg);
        logger_client& operator << (const char msg);
        logger_client& operator << (const int8_t msg);
        logger_client& operator << (const uint8_t msg);
        logger_client& operator << (const int16_t msg);
        logger_client& operator << (const uint16_t msg);
        logger_client& operator << (const int32_t msg);
        logger_client& operator << (const uint32_t msg);
        logger_client& operator << (const int64_t msg);
        logger_client& operator << (const uint64_t msg);
        logger_client& operator << (const double msg);
        logger_client& operator << (const float msg);

        bool flush() { logger_server_.flush(); }

    private:
        logger_server_type& logger_server_;
        char                buffer_[256];
    };

    /*
     * Implementation begins here
     */

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const char* msg)
    {
        if (msg)
            logger_server_.write(msg);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const std::string& msg)
    {
        if (!msg.empty())
            logger_server_.write(msg.c_str(), msg.length());
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const char msg)
    {
        logger_server_.write(&msg, 1u);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const int8_t msg)
    {
        size_t len = int8_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const uint8_t msg)
    {
        size_t len = uint8_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const int16_t msg)
    {
        size_t len = int16_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const uint16_t msg)
    {
        size_t len = uint16_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const int32_t msg)
    {
        size_t len = int32_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const uint32_t msg)
    {
        size_t len = uint32_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const int64_t msg)
    {
        size_t len = int64_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const uint64_t msg)
    {
        size_t len = uint64_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const double msg)
    {
        size_t len = double_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

    template <typename L>
    inline logger_client<L>&
    logger_client<L>::operator << (const float msg)
    {
        size_t len = double_to_string(msg, buffer_);
        logger_server_.write(buffer_, len);
        return *this;
    }

}}// tp::utils

#endif//__tp_utils_logger_client_hxx__
