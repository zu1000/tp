#ifndef __tp_utils_ostream_logger_server_hpp__
#define __tp_utils_ostream_logger_server_hpp__

#include "bucket.hpp"
#include "ring_buffer.hpp"

#include <boost/thread.hpp>

#include <ostream>

namespace tp {
namespace utils {

    template <size_t BS, size_t RS>
    class ostream_logger_server
    {
    public:

        /*
         * Types
         */
        enum
        {
            bucket_size = BS,
            ring_buffer_size = RS
        };

        typedef bucket<BS>                    bucket_type;
        typedef ring_buffer<bucket_type, RS>  ring_buffer_type;

        /*
         * Constructor
         * input
         *      os: ostream
         *      interal: logging interval in milliseconds
         */
        ostream_logger_server(std::ostream& os, size_t interval_in_ms = 1);
        ~ostream_logger_server();

        /*
         * write data to server
         * input
         *    @data: \0 ended data, this is only useful for string
         */
        bool write(const char* data);

        /*
         * write data to server
         * input
         *    @data: input data
         *    @size: size of data
         */
        bool write(const char* data, size_t size);

        /*
         * flush the data in buffer
         */
        bool flush() { return ring_buffer_.flush(); }

    private:
        void run();

    private:
        std::ostream&    os_;
        ring_buffer_type ring_buffer_;
        bool             running_;
        size_t           interval_in_ms_;
        boost::thread    thread_;
    };

    /*
     * Implemenetation beings here
     */

    template <size_t BS, size_t RS>
    ostream_logger_server<BS, RS>::ostream_logger_server(
                                            std::ostream& os,
                                            size_t interval_in_ms)
        : os_(os)
        , running_(true)
        , interval_in_ms_(interval_in_ms*1000)
        , thread_(boost::bind(&ostream_logger_server<BS, RS>::run, this))
    {}

    template <size_t BS, size_t RS>
    ostream_logger_server<BS, RS>::~ostream_logger_server()
    {
        flush();
        running_ = false;
        if (thread_.joinable())
            thread_.join();
    }

    template <size_t BS, size_t RS>
    bool
    ostream_logger_server<BS, RS>::write(const char* data)
    {
        return ring_buffer_.put(data);
    }

    template <size_t BS, size_t RS>
    bool
    ostream_logger_server<BS, RS>::write(const char* data, size_t size)
    {
        return ring_buffer_.put(data, size);
    }

    template <size_t BS, size_t RS>
    void
    ostream_logger_server<BS, RS>::run()
    {
        do
        {
            // sleep a little
            usleep(interval_in_ms_);

            do
            {
                const typename ring_buffer_type::bucket_ptr_type&
                ptr = ring_buffer_.get();

                if (!ptr) break;

                os_ << ptr->data();

            }while (1);

        }while(running_);
    }

}} // tp::utils

#endif//__tp_utils_ostream_logger_server_hpp__
