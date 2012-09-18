#ifndef __tp_comm_io_impl_udp_acceptor_hpp__
#define __tp_comm_io_impl_udp_acceptor_hpp__

#include "peer.hpp"

#include <comm/io/impl/acceptor.hpp>

#include <boost/asio.hpp>

namespace tp {
namespace comm {
namespace service {

    class service;

}}} // tp::comm::service

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    class acceptor : public impl::acceptor
    {
    public:
        acceptor(
                boost::asio::io_service& io,
                impl::acceptor_notifier& notifier,
                const service::service& service);

    private:
        void start_accept();

        void handle_accept(
                boost::shared_ptr<peer> peer,
                const boost::system::error_code& error,
                std::size_t bytes_transferred);

    private:
        boost::asio::ip::udp::socket socket_;
        char            r_buffer_[buffer_size];
    };

}}}}} // tp::comm::io::impl::udp

#endif//__tp_comm_io_impl_udp_acceptor_hpp__
