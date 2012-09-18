#ifndef __TP_WebApp_test_client_h__
#define __TP_WebApp_test_client_h__

#include <web/external_event_manager.hpp>

#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WStandardItemModel>
#include <Wt/WText>

namespace TP {
namespace WebApp {

class TestClient : public Wt::WApplication
{
public:
    TestClient(
            const Wt::WEnvironment& env,
            Web::ExternalEventManager& extern_event_manager);

    ~TestClient();

    void handleMessageEvent(const Web::EventPtr& event);

    void createChart();

private:
    Web::ExternalEventManager& external_event_manager_;
    Wt::WText text_;
    Wt::WStandardItemModel* model_;
};

}}

#endif//__TP_WebApp_test_client_h__
