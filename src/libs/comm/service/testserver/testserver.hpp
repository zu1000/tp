#ifndef __tp_comm_service_testserver_hpp__
#define __tp_comm_service_testserver_hpp__

#include <comm/codec/binary/codec.hpp>
#include <comm/codec/util/byte_order_policy.hpp>

#include <comm/protocol/internal/policy.hpp>

#include <comm/service/serializer.hpp>
#include <comm/service/communicator.hpp>
#include <comm/service/manager.hpp>

#include <boost/thread.hpp>
#include <boost/random.hpp>

namespace tp {
namespace comm {
namespace service {
namespace testapp {

    typedef serializer<codec::util::byte_order_policy> serializer;
    typedef codec::binary::codec<protocol::internal::policy> codec;

    typedef manager<codec, serializer, communicator> manager;

    class testapp : public event_listener
    {
    public:
        testapp()
            : service_(
                    0, service::Multicast, "239.0.32.4", 44332, true, "127.0.0.1")
            , manager_(service_, *this)
        {
        }

        void service_ready  (const service&)
        {
            std::cout << "service_added" << std::endl;
        }

        void service_changed(const service&)
        {
            std::cout << "service_changed" << std::endl;
        }

        void service_deleted(const service_id&)
        {
            std::cout << "service_deleted" << std::endl;
        }

        void
        create_service()
        {
            service_ptr s_ptr(
                    new service(999, service::TCP, "172.0.0.1", 8000, true));
            manager_.publish(s_ptr);
        }

    private:
        service service_;
        manager manager_;
    };

}}}}//tp::comm::service::testapp;

#endif//__tp_comm_service_testserver_hpp__
