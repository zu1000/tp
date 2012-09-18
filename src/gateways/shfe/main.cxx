#include "application.hpp"
#include "session.hpp"
#include "exchange_md_handler.hpp"

int main(int argc, const char** argv)
{
    tp::gateways::shfe::application app;
    if (!app.initialize(argc, argv))
        return 1;
    app.run();
}
