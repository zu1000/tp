#include "external_event_manager.hpp"

#include <Wt/WServer>

using namespace Wt;

namespace TP {
namespace Web {

ExternalEventManager::ExternalEventManager(Wt::WServer& server)
    : state_(STOP)
    , server_(server)
{
}

ExternalEventManager::~ExternalEventManager()
{}

bool
ExternalEventManager::addExternalEventListener(
        EventType type, const ExternalEventListener& listener)
{
    ExternalEventListeners& listeners = listener_map_[type];
    return listeners.insert(listener).second;
}

bool
ExternalEventManager::delExternalEventListener(
        EventType type, const std::string& sessionId)
{
    ExternalEventListeners& listeners = listener_map_[type];
    ExternalEventListeners::iterator it = listeners.begin();

    // TODO: elimate the iteration???
    for (; it != listeners.end(); ++it)
    {
        if (it->first == sessionId)
        {
            listeners.erase(it);
            return true;
        }
    }

    return false;
}

void
ExternalEventManager::triggerEvent(const EventPtr& event)
{
    EventType type = event->type();

    ExternalEventListeners& listeners = listener_map_[type];

    ExternalEventListeners::iterator it = listeners.begin();

    while (it != listeners.end())
    {
        server_.post(it->first, boost::bind(it->second, event));
        it++;
    }
}

}}//TP::Web
