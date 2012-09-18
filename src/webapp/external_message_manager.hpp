#ifndef __WP_WebApp_external_message_manager_h__
#define __WP_WebApp_external_message_manager_h__

#include <web/external_event_manager.hpp>

#include <boost/thread.hpp>

namespace TP {
namespace WebApp {

class ExternalMessageManager : public Web::ExternalEventManager
{
public:
    ExternalMessageManager(Wt::WServer& server);

    ~ExternalMessageManager() {}

    virtual bool start();

    virtual bool stop();

private:
    void exec();

private:
    boost::scoped_ptr<boost::thread> thread_ptr_;
    bool to_stop_;
};

}}

#endif//__WP_WebApp_external_message_manager_h__
