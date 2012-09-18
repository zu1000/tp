#ifndef __TP_Web_external_event_h__
#define __TP_Web_external_event_h__


#include <boost/shared_ptr.hpp>

extern "C" {
#include <stdint.h>
}

namespace TP {
namespace Web {

template <typename EventDataType, EventType TypeValue>
class ExternalEvent : public Event
{
public:
    typedef boost::shared_ptr<EventDataType> EventDataTypePtr;

    explicit ExternalEvent(const EventDataTypePtr& event_data_ptr);
    explicit ExternalEvent(const EventDataType* event_data_ptr);
    virtual EventType type() { return type_; }
    const EventDataType&  getEventData() const { return *event_data_ptr_; }

private:
    EventDataTypePtr event_data_ptr_;
    static EventType type_;
};

template<typename EventDataType, EventType TypeValue>
EventType ExternalEvent<EventDataType, TypeValue>::type_(TypeValue);

template<typename EventDataType, EventType TypeValue>
ExternalEvent<EventDataType, TypeValue>::ExternalEvent(
        const EventDataTypePtr& event_data_ptr)
    : event_data_ptr_(event_data_ptr)
{
}

template<typename EventDataType, EventType TypeValue>
ExternalEvent<EventDataType, TypeValue>::ExternalEvent(const EventDataType* event_data_ptr)
    : event_data_ptr_(event_data_ptr)
{
}

}}//TP::Web

#endif//__TP_Web_external_event_h__
