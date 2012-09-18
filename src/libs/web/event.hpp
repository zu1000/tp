#ifndef __TP_Web_event_h__
#define __TP_Web_event_h__

#include <boost/smart_ptr.hpp>

namespace TP {
namespace Web {

enum EventType
{
    ExtMessageEvent,
    Unknown
};


class Event
{
public:
    // Base holder of all events, must have virtual destructor.
    virtual ~Event() = 0;

    virtual EventType type() = 0;
};

typedef boost::shared_ptr<Event> EventPtr;

}} // TP::Web

#endif//__TP_Web_event_h__
