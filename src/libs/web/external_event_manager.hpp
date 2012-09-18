#ifndef __TP_Web_external_event_manager_h__
#define __TP_Web_external_event_manager_h__

#include "event.hpp"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <set>
#include <map>
#include <vector>

extern "C" {
#include <stdint.h>
}

namespace Wt
{
class WServer;
}

namespace TP {
namespace Web {

typedef boost::function<void (const EventPtr)>       ExternalEventHandler;
typedef std::pair<std::string, ExternalEventHandler> ExternalEventListener;

inline
bool operator < (const ExternalEventListener& l, const ExternalEventListener& r)
{
    return l.first < r.first;
}

typedef std::set<ExternalEventListener>              ExternalEventListeners;
typedef std::map<EventType, ExternalEventListeners>  Event2ListenerMap;

class ExternalEventManager
{
public:
    enum State
    {
        RUN,
        STOP
    };

    ExternalEventManager(Wt::WServer& server);

    virtual ~ExternalEventManager() = 0;

    bool
    addExternalEventListener(
            EventType type,
            const ExternalEventListener& listener);

    bool
    delExternalEventListener(
            EventType type,
            const std::string& sessionId);

    virtual bool start() = 0;

    virtual bool stop()  = 0;

    State& state() { return state_; }

protected:
    void state(State state) { state_ = state; }
    void triggerEvent(const EventPtr& event_ptr);

protected:
    State state_;
    Wt::WServer& server_;
    Event2ListenerMap listener_map_;
};

typedef boost::shared_ptr<ExternalEventManager> ExternalEventManagerPtr;

}}//TP::Web

#endif//__TP_Web_external_connection_manager_h__
