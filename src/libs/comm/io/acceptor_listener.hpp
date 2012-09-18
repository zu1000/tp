#ifndef __tp_comm_io_acceptor_listener_hpp__
#define __tp_comm_io_acceptor_listener_hpp__

namespace tp {
namespace comm {
namespace io {

    class peer;

    class acceptor_listener
    {
    public:
        virtual bool handle_new_peer(
                boost::shared_ptr<peer>) = 0;
    };

}}} // tp::comm::io
#endif//__tp_comm_io_acceptor_listener_hpp__
