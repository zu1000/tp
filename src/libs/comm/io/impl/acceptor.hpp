#ifndef __tp_comm_io_impl_acceptor_hpp__
#define __tp_comm_io_impl_acceptor_hpp__

namespace tp {
namespace comm {
namespace io {
namespace impl {

    class acceptor_notifier;

    class acceptor
    {
    public:
        acceptor(
                acceptor_notifier& notifier);

        virtual ~acceptor() = 0;

    protected:
        acceptor_notifier& notifier_;
    };

}}}}//tp::comm::io::impl

#endif//__tp_comm_io_impl_acceptor_hpp__
