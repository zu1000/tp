#include "external_message_manager.hpp"
#include "message_event.hpp"

#include <stdexcept>

using namespace TP::Web;

namespace TP {
namespace WebApp {

ExternalMessageManager::ExternalMessageManager(Wt::WServer& server)
    : ExternalEventManager(server)
    , to_stop_(false)
{
}

bool
ExternalMessageManager::start()
{
    if (state() == RUN)
        return true;

    try {
        to_stop_ = false;
        thread_ptr_.reset(
                new boost::thread(boost::bind(&ExternalMessageManager::exec, this)));

        state(RUN);
        return true;
    }
    catch(std::exception& ex)
    {
        return false;
    }

}

bool
ExternalMessageManager::stop()
{
    if (state() == STOP)
        return true;

    to_stop_ = true;
    thread_ptr_->join();

    state(STOP);

    return true;
}

void
ExternalMessageManager::exec()
{
    uint32_t count = 0;
    while (!to_stop_)
    {
        if (!listener_map_.empty())
        {
            std::stringstream ss;
            ss << "Message: " << count;

            MessagePtr message(new Message(ss.str()));
            EventPtr event(new ExternalMessageEvent(message));
            triggerEvent(event);
            count ++;
        }

        sleep(1);
    }
}

}}
