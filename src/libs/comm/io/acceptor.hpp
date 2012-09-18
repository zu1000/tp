#ifndef __tp_comm_io_acceptor_hpp__
#define __tp_comm_io_acceptor_hpp__

#include "handler.hpp"
#include "impl/acceptor_notifier.hpp"

#include <comm/service/service.hpp>

#include <boost/asio/io_service.hpp>

namespace tp {
namespace comm {
namespace io {

namespace impl {
    class acceptor;
}

    class acceptor_listener;

    class acceptor
    {
    public:
        acceptor(
                boost::asio::io_service& io,
                const service::service& service,
                const peer_handler& p_handler);

    private:
        boost::shared_ptr<impl::acceptor> acceptor_;
        impl::acceptor_notifier notifier_;
    };

}}} // tp::comm::io

#endif//__tp_comm_io_acceptor_hpp__
