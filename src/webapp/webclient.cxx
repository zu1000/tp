#include "test_client.hpp"
#include "external_message_manager.hpp"

#include <Wt/WEnvironment>
#include <Wt/WServer>

using namespace Wt;
using namespace TP::Web;

WApplication* createApplication(
        const WEnvironment& env, ExternalEventManager& manager)
{
    return new TP::WebApp::TestClient(env, manager);
}

int main(int argc, char**argv)
{
    WServer server(argv[0]);
    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

    TP::WebApp::ExternalMessageManager manager(server);

    server.addEntryPoint(Wt::Application,
                         boost::bind(createApplication, _1, boost::ref(manager)));

    if (server.start())
    {
        std::cerr << "Shutting down: (signal = " <<
            WServer::waitForShutdown() << ")" << std::endl;
        manager.stop();
        server.stop();
    }

    return 0;
}
