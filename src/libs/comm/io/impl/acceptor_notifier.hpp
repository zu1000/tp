#ifndef __tp_comm_io_impl_acceptor_notifier_hpp__
#define __tp_comm_io_impl_acceptor_notifier_hpp__

#include <comm/io/handler.hpp>

#include <boost/smart_ptr.hpp>

namespace tp {
namespace comm {
namespace io {
namespace impl {

    class peer;

    class acceptor_notifier
    {
    public:
        acceptor_notifier(
                    const peer_handler& p_handler);

        void notifiy_new_peer(
                boost::shared_ptr<peer> peer);
    private:
        io::peer_handler p_handler_;
    };

}}}}//tp::comm::io::impl

#endif//__tp_comm_io_impl_acceptor_notifier_hpp__
