#ifndef __TP_WebApp_dummy_event_h__
#define __TP_WebApp_dummy_event_h__

#include <web/external_event.hpp>

namespace TP {
namespace WebApp {

class Message
{
public:
    explicit Message(const std::string& message)
        : message_(message)
    {}

    const std::string& message() const
    {
        return message_;
    }

private:
    std::string message_;
};

typedef boost::shared_ptr<Message> MessagePtr;

typedef Web::ExternalEvent<Message, Web::ExtMessageEvent> ExternalMessageEvent;

}}

#endif//__TP_WebApp_dummy_event_h__
