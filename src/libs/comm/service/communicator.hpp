/*
 * The communicator will have only one socket, the socket will be incharge of
 * receive the data on the multicats channel and send the data to the same
 * multicast channel as well.
 */

#ifndef __tp_comm_serivce_communicator_h__
#define __tp_comm_service_communicator_h__

#include <comm/io/handler.hpp>
#include <comm/io/sender_receiver.hpp>

#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>

namespace tp {
namespace comm {
namespace service {

    static const boost::posix_time::seconds null_seconds(0);
    static const boost::posix_time::seconds default_seconds(1);

    class communicator : public boost::noncopyable
    {
    public:
        communicator(
                const service& service,
                const io::receive_handler& r_handler,
                const io::error_handler&   e_handler,
                const io::timer_handler&   t_handler,
                const boost::posix_time::seconds& seconds = default_seconds);

        ~communicator();

        std::size_t send(
                const io::const_buffer& buffer, io::error_code& ec)
        {
            return sender_receiver_.send(buffer, ec);
        }

        bool start();

        bool stop();

        void start_timer()
        {
            timer_.expires_from_now(expire_seconds_);
            timer_.async_wait(t_handler_);
        }

    private:

        void event_loop()
        {
            io_.run();
        }

    private:
        boost::asio::io_service          io_;
        io::sender_receiver              sender_receiver_;
        boost::asio::deadline_timer      timer_;
        io::timer_handler                t_handler_;
        boost::scoped_ptr<boost::thread> event_loop_thread_ptr_;
        boost::posix_time::seconds       expire_seconds_;
    };

}}}//tp::comm::service

#endif//__tp_comm_communicator_h__
