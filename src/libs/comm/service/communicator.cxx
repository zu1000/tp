#include "communicator.hpp"

#include "service.hpp"

#include <boost/bind.hpp>

namespace tp {
namespace comm {
namespace service {

    using namespace boost::asio;

    communicator::communicator(
            const service& service,
            const io::receive_handler& r_handler,
            const io::error_handler&   e_handler,
            const io::timer_handler&   t_handler,
            const boost::posix_time::seconds& seconds)
        : io_()
        , sender_receiver_(service, io_, r_handler, e_handler)
        , timer_(io_)
        , t_handler_(t_handler)
        , expire_seconds_(seconds)
    {
        if (seconds == null_seconds)
            timer_.expires_from_now(default_seconds);
        else
            timer_.expires_from_now(seconds);

        timer_.async_wait(t_handler);
    }

    communicator::~communicator()
    {
        stop();
    }

    bool
    communicator::start()
    {
        if (event_loop_thread_ptr_)
            return false;

        event_loop_thread_ptr_.reset(
                new boost::thread(&communicator::event_loop, this));

        return true;
    }

    bool
    communicator::stop()
    {
        if (event_loop_thread_ptr_)
        {
            io_.stop();
            event_loop_thread_ptr_->join();
            event_loop_thread_ptr_.reset(NULL);
            return true;
        }
        else
            return false;
    }

}}}//tp::comm::service
