#ifndef __tp_comm_io_impl_udp_peer_hpp__
#define __tp_comm_io_impl_udp_peer_hpp__

#include <comm/io/impl/peer.hpp>

#include <boost/asio.hpp>

namespace tp {
namespace comm {
namespace io {
namespace impl {
namespace udp {

    class peer : public impl::peer
    {
        friend class acceptor;
    public:
        peer(
                boost::asio::io_service& io);

        void init_peer(
                const receive_handler& r_handler,
                const error_handler& e_handler);

        std::size_t send(
                const const_buffer& buffer,
                error_code& error);

    private:
        void async_receive_handler(
                const boost::system::error_code& ec,
                std::size_t bytes_transferred);

    private:
        boost::asio::ip::udp::socket   socket_;
        boost::asio::ip::udp::endpoint remote_endpoint_;
        receive_handler r_handler_;
        error_handler   e_handler_;
        char            r_buffer_[buffer_size];
        std::size_t     last_bytes_transferred_;
    };

}}}}} // tp::comm::io::impl::udp
#endif//__tp_comm_io_impl_udp_peer_hpp__
