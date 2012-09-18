/*
 * Service listner is a class that would be interested in the events happening
 * in the service manager.
 *
 * This is a pure interface, which should be implemented by the real listener
 *
 */

#ifndef __tp_comm_service_event_listener_h__
#define __tp_comm_service_event_listener_h__

#include "service.hpp"

namespace tp {
namespace comm {
namespace service {

    class event_listener : public boost::noncopyable
    {
    public:
        virtual ~event_listener() = 0;
        virtual void service_ready  (const service& service) = 0;
        virtual void service_changed(const service& service) = 0;
        virtual void service_deleted(const service_id& id) = 0;
    };

}}}//tp::comm::service

#endif//__tp_comm_service_event_listener_h__
